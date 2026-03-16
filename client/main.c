#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 4096
#define PORT 8000

int main() {

  int socket_fd;
  struct sockaddr_in sockaddr;

  // cleating a socket
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  // fail check
  if (socket_fd < 0) {
    printf("error while creating socket\n");
  }

  // setting up address info
  sockaddr.sin_port = htons(PORT);
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  // conneting to the socket and checing for connection error
  if (connect(socket_fd, (const struct sockaddr *)&sockaddr, sizeof sockaddr) <
      0) {
    printf("failed to connet to the socket...\n");
    return -1;
  }

  // creating a request header
  char *request = "GET / HTTP/1.1\r\n"
                  "Host: tcp-9tdi.onrender.com\r\n"
                  "Accept: text/html\r\n"
                  "Connection: keep-alive\r\n"
                  "\r\n\r\n";

  // sending the request to server
  send(socket_fd, request, strlen(request), 0);

  // reading the response from server
  char buffer[BUFFER_SIZE];
  while (read(socket_fd, buffer, BUFFER_SIZE) > 0) {
    printf("%s", buffer);
  }

  return 0;
}
