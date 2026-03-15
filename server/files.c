#include "files.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>

char *read_from_file(char *file_name, int *size) {
  FILE *f = fopen(file_name, "rb"); // opening file in readonly mode

  if (f == NULL) { // exiting if file didn't open
    printf("unable to open file");
    return NULL;
  }

  fseek(f, 0, SEEK_END); // going to the end of file
  *size = ftell(f);      // getting position and storing it as size
  fseek(f, 0, SEEK_SET); // going to beginning of the file

  char *data = malloc(*size); // allocating space to store data
  if (data == NULL) {         // exiting if allocation failed
    fclose(f);
    return NULL;
  }

  fread(data, 1, *size, f); // reading the entire file

  fclose(f); // closing file after user

  return data;
}

int find_public_file(const char *file_name, char *path) {
  char absolutePath[PATH_MAX];

  char file_path[PATH_MAX] = "public";
  strcat(file_path,file_name);
  char *full_path = realpath(file_path, absolutePath);

  if (full_path != NULL && strstr(full_path, "public") != NULL) {
    strcpy(path, absolutePath);
    return 0;
  }

  return -1;
}


const char* get_file_type(const char *path) {
  const char *ext = strrchr(path, '.');  // getting file extension

  // returning content type according to the extension
  if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0) return "text/html";
  if (strcmp(ext, ".css")  == 0) return "text/css";
  if (strcmp(ext, ".js")   == 0) return "application/javascript";
  if (strcmp(ext, ".json") == 0) return "application/json";
  if (strcmp(ext, ".png")  == 0) return "image/png";
  if (strcmp(ext, ".jpg")  == 0 || strcmp(ext, ".jpeg") == 0) return "image/jpeg";
  if (strcmp(ext, ".gif")  == 0) return "image/gif";
  if (strcmp(ext, ".svg")  == 0) return "image/svg+xml";
  if (strcmp(ext, ".ico")  == 0) return "image/x-icon";
  if (strcmp(ext, ".pdf")  == 0) return "application/pdf";
  if (strcmp(ext, ".txt")  == 0) return "text/plain";
  if (strcmp(ext, ".xml")  == 0) return "application/xml";
  if (strcmp(ext, ".mp4")  == 0) return "video/mp4";
  if (strcmp(ext, ".mp3")  == 0) return "audio/mpeg";
  if (strcmp(ext, ".woff2")== 0) return "font/woff2";

  return "application/octet-stream";  // in case nothing works
}