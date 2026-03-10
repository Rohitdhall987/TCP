#include "response.h"
#include "files.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

void send_response(int fd, char *file_name, char *status, char *type) {

  int size;
  char *data = read_from_file(
      file_name, &size); // reading data from file and stong data size

  char header[256];
  snprintf(header, sizeof header, // creating response header
           "HTTP/1.1 %s\r\n"
           "Content-Type: %s\r\n"
           "Content-Length: %d\r\n"
           "\r\n",
           status, type, size);

  send(fd, header, strlen(header), 0); // sending header to client;
  send(fd, data, size, 0);             // sending body to client
}
