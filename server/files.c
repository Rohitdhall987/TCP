#include "files.h"

char *read_from_file(char *file_name, int *size) {
  FILE *f = fopen(file_name, "r"); // opening file in readonly mode

  if (f == NULL) { // exiting if file didn't open
    printf("unable to open file");
    return NULL;
  }

  fseek(f, 0, SEEK_END); // going to the end of file
  *size = ftell(f);      // getting position and storing it as size
  fseek(f, 0, SEEK_SET); // going to to begginig of the file

  char *data = malloc(*size); // acllocating space to store data
  if (data == NULL) {         // exiting if allocation failed
    fclose(f);
    return NULL;
  }

  fread(data, 1, *size, f); // reading the entire file

  fclose(f); // closing file after user

  return data;
}
