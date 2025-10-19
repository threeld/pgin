#include "server.h"
#include "colors.h"
#include "errno.h"

// Global flag to control the main server loop.
// `volatile`: ensures compiler reads the variable each time from memory,
//             'cause signal handler might change the value of the variable.
// `sig_atomic_t` guarantees that setting the flag is an atomic operation.
volatile sig_atomic_t keep_running = 1; 

// signal handler, for SIGINT (Ctrl+C).
// sets the flag to 0 to gracefully stop the server.
void sigint_handler(int sig)
{
  (void)sig; // suppress unused parameter warning
  keep_running = 0;
  printf(BLUE"Shutting down the server...\n"RESET);
}

int main()
{
  signal(SIGINT, sigint_handler);

  // Create a socket
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror(RED"Socket creation failed"RESET);
        exit(EXIT_FAILURE);
    }
    printf(GREEN "Socket created successfully\n" RESET);

  // Allow reuse of the same port (fixes “Address already in use”)
  int opt = 1; // Option value is 1 to enable the option 
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) 
  // SOL_SOCKET enables the socket level API.
  // SO_REUSEADDR is the option-name which allows to reuse the port immediately instead of waiting it to be free.
  // &opt -> address to option value (1 -> On, 0 -> Off)
  // sizeof(opt) -> converts size of opt value in bytes
  {
    perror(RED"Reusing Address failed"RED);
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  // prepare the sockaddr_in structure
  struct sockaddr_in server;
  server.sin_family = AF_INET;         // IPv4
  server.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP
  server.sin_port = htons(8080);       // Port 8080

  // bind the socket to the address and port
  if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) < 0)
  {
    perror(RED"Bind failed\n"RESET);
    close(server_socket);
    exit(EXIT_FAILURE);
  }
  printf(GREEN "Bind done on port: ");
  printf(BOLD UNDERLINE "0.0.0.0:8080\n" RESET);

  // listens for incoming connections
  listen(server_socket, 3); // Queue up to 3 connections
  printf(GREEN"Waiting for incoming connections...\n\n"RESET);

  while (keep_running)
  {
    // Accept an incoming connection
    struct sockaddr_in client;
    int client_len = sizeof(client);
    // issue is here, w/ accept function.
    // the accept halts the program, and wait for a new connection.
    // the while condition is only checked after the accept is done.
    // thus, the while codition is only checked after a new connection is made.
    //
    // even if the keep_running=0, the while loop will exit only after the accept is done.
    // and the accept will be done only after it receives a new connection.
    int client_socket = accept(server_socket, (struct sockaddr *)&client,
                               (socklen_t *)&client_len);

    if (client_socket < 0)
    {
       if (errno == EINTR) {            // Interrupted by Ctrl+C
        if (!keep_running)           // Signal handler asked us to quit
            break;                   // Break out of loop cleanly
        else
            continue;                // Ignore other spurious signals
    }

    perror("Accept failed");         // Real error — not from a signal
    break;
    }
    
    printf(CYAN"Connection accepted!\n" RESET);

    // receive data from client
    char client_message[2000];
    size_t read_size = recv(client_socket, client_message, 2000, 0);

    if (read_size > 0)
    {
      printf("Client sent: %s\n", client_message);

      // sends response to client
      char *response = "HTTP/1.1 200 OK\r\nContent-Type: "
                       "text/html\r\n\r\n<h1>Welcome to pgin!</h1>";
      send(client_socket, response, strlen(response), 0);
      printf(BOLD"Response sent\n\n" RESET);
    }
    close(client_socket);
  }
  close(server_socket);

  return 0;
}
