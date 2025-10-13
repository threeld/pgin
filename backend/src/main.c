#include "server.h"

// Global flag to control the main server loop.
// `volatile`: ensures compiler reads the variable each time from memory,
//             'cause signal handler might change the value of the variable.
// `sig_atomic_t` guarantees that setting the flag is an atomic operation.
volatile sig_atomic_t kr = 1; // kr: keep running

// signal handler, for SIGINT (Ctrl+C).
// sets the flag to 0 to gracefully stop the server.
void sigint_handler(int sig) {
  (void)sig; // suppress unused parameter warning
  kr = 0;
}

int main() {
  signal(SIGINT, sigint_handler);

  // create a socket
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    perror("Could not create socket\n");
    return 1;
  }
  printf("Socket created\n");

  // prepare the sockaddr_in structure
  struct sockaddr_in server;
  server.sin_family = AF_INET;         // IPv4
  server.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP
  server.sin_port = htons(8080);       // Port 8080

  // bind the socket to the address and port
  if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("Bind failed\n");
    close(server_socket);
    return 1;
  }
  printf("Bind done on port 8080\n");

  // listens for incoming connections
  listen(server_socket, 3); // Queue up to 3 connections
  printf("Waiting for incoming connections...\n");

  while (kr) {
    // 5. Accept an incoming connection
    struct sockaddr_in client;
    int client_len = sizeof(client);
    // issue is here, w/ accept function.
    // the accept halts the program, and wait for a new connection.
    // the while condition is only checked after the accept is done.
    // thus, the while codition is only checked after a new connection is made.
    //
    // even if the kr=0, the while loop will exit only after the accept is done.
    // and the accept will be done only after it receives a new connection.
    int client_socket = accept(server_socket, (struct sockaddr *)&client,
                               (socklen_t *)&client_len);

    if (client_socket < 0) {
      perror("Accept failed\n");
      close(client_socket);
      return 1;
    }
    printf("Connection accepted\n");

    // receive data from client
    char client_message[2000];
    size_t read_size = recv(client_socket, client_message, 2000, 0);

    if (read_size > 0) {
      printf("Client sent: %s\n", client_message);

      // sends response to client
      char *response = "HTTP/1.1 200 OK\r\nContent-Type: "
                       "text/html\r\n\r\n<h1>Welcome to pgin!</h1>";
      send(client_socket, response, strlen(response), 0);
      printf("Response sent\n");
    }
    close(client_socket);
  }
  close(server_socket);

  return 0;
}
