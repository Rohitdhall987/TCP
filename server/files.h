#pragma once

char *read_from_file(char *file_name, int *size);
int find_public_file(const char *file_name, char *path);
const char* get_file_type(const char *path);
