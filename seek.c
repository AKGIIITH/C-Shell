#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include "global.h"
#include "utils.h"
#include "seek.h"
#include "utils.h"

#define COLOR_GREEN "\033[0;32m"
#define COLOR_BLUE "\033[0;34m"
#define COLOR_RESET "\033[0m"


void seek_directory(const char *target_directory, const char *search, int other[3], int *match_count, char matched_paths[MAXPATH][MAXPATH]) {
    DIR *dir;
    struct dirent *entry;
    char path[MAXPATH];

    if ((dir = opendir(target_directory)) == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        int is_file = (entry->d_type == DT_REG);
        int is_directory = (entry->d_type == DT_DIR);

        if ((other[0] && !is_directory) || (other[1] && !is_file) || (strcmp(entry->d_name, search) != 0)) {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", target_directory, entry->d_name);
        relative_directory(path,home);
        print_matches(path, is_file, is_directory);

        if (other[2] && (*match_count < MAXPATH)) {
            strncpy(matched_paths[(*match_count)++], path, sizeof(matched_paths[0]));
        }

        if (is_directory && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            seek_directory(path, search, other, match_count, matched_paths); // Recursive call
        }
    }
    closedir(dir);
}

// Function to print matched paths with color codes
void print_matches(const char *path, int is_file, int is_directory) {
    if (is_directory) {
        printf(COLOR_BLUE "%s\n" COLOR_RESET, path);
    } else if (is_file) {
        printf(COLOR_GREEN "%s\n" COLOR_RESET, path);
    } else {
        printf("%s\n", path);
    }
}

// Function to print the content of a file
void print_file_content(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        perror("fopen");
        return;
    }

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }
    fclose(file);
}

void seek_command(char *search, char *target_directory, int other[3]) {
    char matched_paths[MAXPATH][MAXPATH];
    int match_count = 0;

    if (target_directory == NULL) {
        target_directory = "."; // Default to current directory if none specified
    }

    seek_directory(target_directory, search, other, &match_count, matched_paths);

    // Handle -e flag and match results
    if (other[2] && match_count == 1) {
        struct stat statbuf;
        if (stat(matched_paths[0], &statbuf) == 0) {
            if (S_ISDIR(statbuf.st_mode)) {
                if (chdir(matched_paths[0]) == 0) {
                    printf("This is a new folder!\n");
                } else {
                    perror("chdir");
                }
            } else if (S_ISREG(statbuf.st_mode)) {
                print_file_content(matched_paths[0]);
            }
        } else {
            perror("stat");
        }
    } else if (other[2] && match_count == 0) {
        printf("No match found!\n");
    }
}

void seek(int argc, char *argv[]) {
    int other[3];
    other[0] = 0; //-d
    other[1] = 0; //-f
    other[2] = 0; //-e
    
    char target_directory[1024];
    getcwd(target_directory, sizeof(target_directory));
    
    char search[1024] = "";
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && strlen(argv[i])!=1) {
            for (int j = 1; j < strlen(argv[i]); j++) {
                if (argv[i][j] == 'd') {
                    other[0] = 1;
                } 
                else if (argv[i][j] == 'f') {
                    other[1] = 1;
                }
                else if (argv[i][j] == 'e') {
                    other[2] = 1;
                }
            }
        }
        else{
            if(strcmp(search,"")==0){
                strcpy(search,argv[i]);
            }
            else{
                resolve_path(argv[i],target_directory);  
            }
        }
    }
    if (strlen(search) > 0) {
        // printf("%s %s %d %d %d\n", search, target_directory, other[0], other[1], other[2]);
        seek_command(search, target_directory,other);
    }
}

