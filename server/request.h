#pragma once

typedef struct request_node {
  int client_fd;
  struct request_node *next;
} request_node_t;

typedef struct {
  request_node_t *head;
  request_node_t *tail;
} request_queue_t;

void init_queue(request_queue_t *queue);
void add_request(request_queue_t *queue, int c_fd);
int get_request(request_queue_t *queue);

void *handle_requests(void *requests_queue);
