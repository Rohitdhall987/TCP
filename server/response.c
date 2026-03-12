#include "response.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

void send_response(int fd, response_data_t *res) {

  char header[256];
  snprintf(header, sizeof header, // creating response header
           "HTTP/1.1 %s\r\n"
           "Content-Type: %s\r\n"
           "Content-Length: %d\r\n"
           "\r\n",
           res->status, res->type, res->size);

  send(fd, header, strlen(header), 0); // sending header to client
  send(fd, res->data, res->size, 0);   // sending body to client
}
