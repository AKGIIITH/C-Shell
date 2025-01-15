#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include "utils.h"

void print_file_info(char *path, char *name, int long_format) {
    char full_path[1024];
    snprintf(full_path, sizeof(full_path), "%s/%s", path, name);

    struct stat file_stat;
    if (stat(full_path, &file_stat) == -1) {
        perror("stat");
        return;
    }

    if (long_format) {
        printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
        printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
        printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
        printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
        printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
        printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
        printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
        printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
        printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
        printf((file_stat.st_mode & S_IXOTH) ? "x" : "-");

        printf(" %lu", file_stat.st_nlink);

        struct passwd *pw = getpwuid(file_stat.st_uid);
        printf(" %s", pw->pw_name);

        struct group *gr = getgrgid(file_stat.st_gid);
        printf(" %s", gr->gr_name);

        printf(" %5ld", file_stat.st_size);

        char time_buf[80];
        struct tm *time_info;
        time_info = localtime(&file_stat.st_mtime);
        strftime(time_buf, sizeof(time_buf), "%b %d %H:%M", time_info);
        printf(" %s", time_buf);
    }

    if (S_ISDIR(file_stat.st_mode)) {
        printf(" \033[1;34m%s\033[0m\n", name); 
    } else if (file_stat.st_mode & S_IXUSR) {
        printf(" \033[1;32m%s\033[0m\n", name); 
    } else {
        printf(" \033[0;37m%s\033[0m\n", name); 
    }
}

void reveal_dir(char *path, int show_all, int long_format) {
    struct dirent **namelist;
    int n;

    n = scandir(path, &namelist, NULL, alphasort);
    if (n == -1) {
        perror("scandir");
        return;
    }

    for (int i = 0; i < n; i++) {
        if (!show_all && namelist[i]->d_name[0] == '.') {
            free(namelist[i]);
            continue;
            //If Hidden File need to be hidden, remove them they start with .
        }

        print_file_info(path, namelist[i]->d_name, long_format);
        free(namelist[i]);
    }
    free(namelist);
}

void reveal(int argc, char *argv[]) {
    int show_all = 0;
    int long_format = 0;
    
    char path[1024];

    if (argc < 2) {
        getcwd(path, sizeof(path));
    }
    else {
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] == '-' && strlen(argv[i])!=1) {
                for (int j = 1; j < strlen(argv[i]); j++) {
                    if (argv[i][j] == 'a') {
                        show_all = 1;
                    } else if (argv[i][j] == 'l') {
                        long_format = 1;
                    }
                }
            }
            else {
                resolve_path(argv[i], path);
            }
        }
    }
    if (strlen(path) > 0) {
        reveal_dir(path, show_all, long_format);
    }
}
