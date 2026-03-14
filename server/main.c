#include "request.h"
#include "server.h"
#include <bits/pthreadtypes.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int server_fd; // server file descriptor

void handle_signal(int sig) {

  printf("Closing server if open\n");

  if (server_fd > 0)
    close(server_fd); // closing server

  exit(0);
}

int main() {

  struct sigaction sa;
  pthread_t thread_pool[THREADS_COUNT]; // pool to hold all threads
  int status;                // used for checking error while setting up socket
  int opt = 1;               // used in for setting socket option to true
  struct sockaddr_in s_addr; // holds server socket info
  request_queue_t requests_queue; // queue to hold incoming requests

  init_queue(&requests_queue); // initializing the queue

  memset(&sa, 0, sizeof sa);     // setting memory to 0
  sa.sa_handler = handle_signal; // setting signal action function

  sigaction(
      SIGINT, &sa,
      NULL); // calls the function when it recives interactive attenction signal

  server_fd = socket(AF_INET, SOCK_STREAM, 0); // creating a socket

  if (server_fd < 0) { //  exits if error
    perror("Unable to create socket, exiting from the application\n");
    exit(-1);
  }

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) { // trying to forcfully connect to the port
    perror("setsockopt failed");
    exit(EXIT_FAILURE);
  }

  s_addr.sin_family = AF_INET;   // Address family ipv4
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
      50); // ready to listen on socket, can have maximum of 5 requests in queue

  if (status < 0) { // exits on error
    perror("Unable to listen to the socket\n");
    exit(-1);
  }

  // initializing mutex lock
  if (pthread_mutex_init(&lock, NULL) != 0) {
    printf("\n mutex init failed\n");
    return 1;
  }

  // initializing condition variable
  if (pthread_cond_init(&condition, NULL) != 0) {
    printf("\n condtition init failed\n");
    return 1;
  }

  // creating worker threads to handle incoming request
  for (int i = 0; i < THREADS_COUNT; i++) {
    if (pthread_create(&thread_pool[i], NULL, handle_requests,
                       (void *)&requests_queue) != 0) {
      printf("thread creation failed at :%d\n", i);
    }
  }

  while (1) {
    int client_fd; // client file descriptor
    printf("Waiting for connections on port:%d\n", PORT);

    client_fd = accept(server_fd, NULL, NULL); // accepting all requests

    pthread_mutex_lock(
        &lock); // locking the data from being accessed by other threads
    add_request(&requests_queue, client_fd); // adding client in the queue;
    pthread_cond_signal(&condition);         // unblocking threads
    pthread_mutex_unlock(&lock);             // unlocking data for other threads
  }

  return 0;
}
