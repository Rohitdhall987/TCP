#pragma once

typedef struct {
  char *data;
  int size;
  char *status;
  char *type;

} response_data_t;

void send_response(int fd, response_data_t *res);
