#ifndef SEEK_H
#define SEEK_H
#include "global.h"

// Function prototypes
void seek_directory(const char *target_directory, const char *search, int other[3], int *match_count, char matched_paths[MAXPATH][MAXPATH]);
void print_matches(const char *path, int is_file, int is_directory);
void print_file_content(const char *filepath);
void seek_command(char *search, char *target_directory, int other[3]);
void seek(int argc, char *argv[]);

#endif 
