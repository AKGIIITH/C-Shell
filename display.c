#include "display.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include "global.h"
void display_prompt(char* home) {
    char username[256];
    char systemname[256];
    char cwd[1024];

    get_username(username);
    get_systemname(systemname);
    get_current_directory(cwd,home);

    if(strcmp(normal,"NORMAL")==0){
        printf("<%s@%s:%s> ", username, systemname, cwd);
    }
    else{
        printf("<%s@%s:%s %s> ", username, systemname, cwd,normal);
        strcpy(normal,"NORMAL");
    }
    
}
