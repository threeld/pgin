#include "server.h"
#include <signal.h>

volatile sig_atomic_t kr = 1;
void sigint_handler(int sig) {
  if (kr)
    kr = 0;
}

int main() {
  signal(SIGINT, sigint_handler);

  // 1. Create a socket
  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    perror("Could not create socket\n");
    return 1;
  }
  printf("Socket created\n");

  // 2. Prepare the sockaddr_in structure
  struct sockaddr_in server;
  server.sin_family = AF_INET;         // IPv4
  server.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP
  server.sin_port = htons(8080);       // Port 8080

  // 3. Bind the socket to the address and port
  if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("Bind failed\n");
    close(server_socket);
    return 1;
  }
  printf("Bind done on port 8080\n");

  // 4. Listen for incoming connections
  listen(server_socket, 3); // Queue up to 3 connections
  printf("Waiting for incoming connections...\n");

  while (kr) {
    // 5. Accept an incoming connection
    struct sockaddr_in client;
    int client_len = sizeof(client);
    int client_socket = accept(server_socket, (struct sockaddr *)&client,
                               (socklen_t *)&client_len);

    if (client_socket < 0) {
      perror("Accept failed\n");
      close(client_socket);
      return 1;
    }
    printf("Connection accepted\n");

    // 6. Receive data from client
    char client_message[2000];
    size_t read_size = recv(client_socket, client_message, 2000, 0);

    if (read_size > 0) {
      printf("Client sent: %s\n", client_message);

      // 7. Send response to client
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
