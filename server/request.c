#include "request.h"
#include "response.h"
#include "router.h"
#include "server.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void init_queue(request_queue_t *queue) {
  // initializing queue with NULL
  queue->head = NULL;
  queue->tail = NULL;
}

void add_request(request_queue_t *queue, int c_fd) {

  // storing new request in heap
  request_node_t *new_request = malloc(sizeof(request_node_t));
  new_request->client_fd = c_fd;
  new_request->next = NULL;

  // if queue is empty adding the element to head
  if (queue->head == NULL) {
    queue->head = new_request;
    queue->tail = new_request;
    return;
  }

  queue->tail->next = new_request; // adding the element to end
  queue->tail = new_request;       // moving the tail to new end
}

int get_request(request_queue_t *queue) {

  // initially no requests
  int c_fd = -1;

  // if thier is a request getting its client id and freeing the head
  if (queue->head != NULL) {
    c_fd = queue->head->client_fd;

    request_node_t *temp = queue->head;
    queue->head = queue->head->next;

    if (queue->head == NULL)
      queue->tail = NULL;

    free(temp);
  }

  return c_fd;
}

void *handle_requests(void *requests_queue) {
  // working forever so thread doesnt die
  while (1) {

    pthread_mutex_lock(
        &lock); // locking the data so other thread cant access the same data
    int c_fd = get_request(requests_queue); // getting reuest from the queue
    pthread_mutex_unlock(
        &lock); //  unlocking the data for other threads to access

    if (c_fd < 0) { // No work to do start over again
      continue;
    }

    char buffer[BUFFER_SIZE];
    int total = 0;

    // reading request and storing to the buffer
    while (1) {
      int n = read(c_fd, buffer + total, sizeof(buffer) - total - 1);

      if (n <= 0)
        break;

      total += n;
      buffer[total] = '\0';

      if (strstr(buffer, "\r\n\r\n"))
        break;
    }

    // accessing method and route from the incoming request
    char *method = strtok(buffer, " ");
    char *route = strtok(NULL, " ");

    response_data_t res = {0}; //  used in storing response data
    handle_routing(method, route,
                   &res); // setting response data accoring to the request

    send_response(c_fd, &res); // sending response to client

    free(res.data); // freeing allocated memory

    close(c_fd); // closing connection with client
  }

  return NULL;
}
