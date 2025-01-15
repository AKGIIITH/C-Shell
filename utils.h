#include <stddef.h>
#ifndef UTILS_H
#define UTILS_H

void get_username(char *username);
void get_systemname(char *systemname);
void get_current_directory(char *cwd,char* home);
void get_home_directory(char *home);
char* trim_whitespace(char *str);
void relative_directory(char *path,char* home);
void resolve_path(char *arg, char *path);
void update_process_list();
#endif
