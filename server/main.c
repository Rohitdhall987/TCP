#include <netinet/in.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8000
#define BUFFER_SIZE 4096

int server_fd; // server file descriptor

void handle_signal(int sig) {

  printf("Closing server if open\n");

  if (server_fd > 0)
    close(server_fd); // closing server

  exit(0);
}

int main() {

  struct sigaction sa;
  memset(&sa, 0, sizeof sa);
  sa.sa_handler = handle_signal; // setting signal action function

  sigaction(SIGINT, &sa, NULL); // calls the function when it recives
                                // interactive attenction signal
  int status;
  struct sockaddr_in s_addr; // holds server socket info

  server_fd = socket(AF_INET, SOCK_STREAM, 0); // creating a socket

  if (server_fd < 0) { //  exits if error
    perror("Unable to create socket, exiting from the application\n");
    exit(-1);
  }

  s_addr.sin_family = AF_INET;
  s_addr.sin_port = htons(PORT); // converting port from host to network bytes
  s_addr.sin_addr.s_addr = INADDR_ANY; // accepts any address

  status = bind(server_fd, (struct sockaddr *)&s_addr,
                sizeof s_addr); // binding server with provided info

  if (status < 0) { // esits on error
    perror("Unable to bind the socket\n");
    exit(-1);
  }

  status = listen(
      server_fd,
      5); // ready to listen on socket, can have maximum of 5 requests in queue

  if (status < 0) { // exits on error
    perror("Unable to listen to the socket\n");
    exit(-1);
  }

  while (1) {
    int client_fd, n; // client file descriptor and variable to hold read size
    char req[BUFFER_SIZE]; // buffer to hold request info
    printf("Waiting for connections on port:%d\n", PORT);

    client_fd = accept(server_fd, NULL, NULL); // accepting all requests

    while ((n = read(client_fd, req, BUFFER_SIZE - 1)) >
           0) { // reading requst and printing to console
      write(STDOUT_FILENO, req, n);

      req[n] = '\0';

      if (strstr(req, "\r\n\r\n")) // exits the loop when it reaches the end
        break;
    }

    char header[256]; // buffer to  hold header

    const char *body = "<h1>HELLO WORLD</h1>"; // buffer to hold response body
    snprintf(header, sizeof header,            // filling header with data
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html\r\n"
             "Content-Length: %lu\r\n"
             "Accept-Ranges: bytes\r\n"
             "Connection: close\r\n"
             "\r\n",
             strlen(body));

    send(client_fd, header, strlen(header), 0); // sending header to client
    send(client_fd, body, strlen(body), 0);     // sending body to client

    close(client_fd); // closing connection with client
  }

  return 0;
}
