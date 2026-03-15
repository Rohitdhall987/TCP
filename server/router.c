#include "router.h"

#include <stdlib.h>
#include "files.h"
#include "response.h"
#include <string.h>
#include <linux/limits.h>

// helper function for handling get method routes
void handle_get_method(char *route, response_data_t *res);

void handle_routing(char *method, char *route, response_data_t *res) {

  // handling requests according to the method
  if (strcmp(method, "GET") == 0) {

    handle_get_method(route, res); // handling route for get method

  } else {

    const char err_msg[] = "{\"err\": \"method not supported\"}";
    char * data = malloc(strlen(err_msg));
    strcpy(data, err_msg);
    // setting default response
    res->data = data;
    res->size = strlen(res->data);
    res->status = "400 Bad Request";
    res->type = "application/json";
  }
}

void handle_get_method(char *route, response_data_t *res) {

  int size; // stores body size

  // setting response according to the route name
  if (strcmp(route, "/") == 0) {
    res->data = read_from_file("public/index.html", &size);
    res->size = size;
    res->status = "200 OK";
    res->type = "text/html";
  }else if (strcmp(route, "/favicon.ico") == 0) {
    res->data = read_from_file("public/icon.png", &size);
    res->size = size;
    res->status = "200 OK";
    res->type = "image/png";
  } else {
    char * path = malloc(PATH_MAX);
    if (find_public_file(route, path)==0) {
      res->data = read_from_file(path, &size);
      res->size = size;
      res->status = "200 OK";
      res->type = get_file_type(path);
    }else {
      res->data = read_from_file("public/404not_found.html", &size);
      res->size = size;
      res->status = "404 Not Found";
      res->type = "text/html";
    }

    free(path);
  }
}
