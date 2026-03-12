#include "router.h"
#include "files.h"
#include "response.h"
#include <string.h>

// helper function for handling get method routes
void handle_get_method(char *route, response_data_t *res);

void handle_routing(char *method, char *route, response_data_t *res) {

  // handling requests accoring to the method
  if (strcmp(method, "GET") == 0) {

    handle_get_method(route, res); // handling route for get method

  } else {

    // setting default response
    res->data = "{err: method not supported}";
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
  } else if (strcmp(route, "/favicon.ico") == 0) {

    res->data = read_from_file("public/icon.png", &size);
    res->size = size;
    res->status = "200 OK";
    res->type = "image/png";

  } else {
    res->data = read_from_file("public/404not_found.html", &size);
    res->size = size;
    res->status = "404 Not Found";
    res->type = "text/html";
  }
}
