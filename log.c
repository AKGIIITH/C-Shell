#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "execution.h"
#include "log.h"
#include "input.h"
#include "global.h"

#define MAX_LOG_SIZE 15
#define LOG_COMMAND "log"

char* LOG_FILE="command_log.txt";

void read_log() {
    char line[1024];
   
    char cfile_path[1024];
    snprintf(cfile_path, sizeof(cfile_path),"%s/%s",home,LOG_FILE);
    FILE *file = fopen(cfile_path, "r");
    if (file == NULL) {
        perror("fopen");
        return;
    }
    
    int index=0;
    char* LastCommands[MAX_LOG_SIZE];
    while(fgets(line, sizeof(line), file) != NULL){
        LastCommands[index] = (char *)malloc(1024 * sizeof(char));
        if (LastCommands[index] == NULL) {
            perror("malloc");
        }
        else{
            strcpy(LastCommands[index],line);
            index++;
        }        
    }
    while(index>0){
        printf("%s",LastCommands[--index]);
    }
    fclose(file);
}

void purge_log() {
    char cfile_path[1024];
    snprintf(cfile_path, sizeof(cfile_path),"%s/%s",home,LOG_FILE);
    FILE *file = fopen(cfile_path, "w");
    if (file == NULL) {
        perror("fopen");
        return;
    }
    fclose(file);
}

void execute_log_index(int index) {
    char line[1024];
    
    char cfile_path[1024];
    snprintf(cfile_path, sizeof(cfile_path),"%s/%s",home,LOG_FILE);
    FILE *file = fopen(cfile_path, "r");
    if (file == NULL) {
        perror("fopen");
        return;
    }
    int current_index = 1;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (current_index == index) {
            // Execute the command
            handle_input(line);
            fclose(file);
            return;
        }
        current_index++;
    }
    fclose(file);
    fprintf(stderr, "Index out of range\n");
}

void add_to_log(const char *command) {
    if (strstr(command, LOG_COMMAND) != NULL) {
        return;
    }
    char cfile_path[1024];
    snprintf(cfile_path, sizeof(cfile_path),"%s/%s",home,LOG_FILE);
    
    FILE *file = fopen(cfile_path, "r");
    if (file == NULL) {
        file = fopen(cfile_path, "w");
        if (file == NULL) {
            perror("fopen");
            return;
        }
        fclose(file);
        file = fopen(cfile_path, "r");
    }

    char *file_name = "temp_log.txt";
    char file_path[1024];
    snprintf(file_path, sizeof(file_path), "%s/%s",home, file_name);
    
    FILE *temp = fopen(file_path, "w");
    if (temp == NULL) {
        perror("fopen");
        fclose(file);
        return;
    }
    // Check if the command is the same as the last command
    int check=1;
    char last_command[1024];
    if(fgets(last_command, sizeof(last_command), file)!=NULL){
        check=0;
        if (strncmp(command, last_command,strlen(command)) == 0) {
            // printf("%s %s %d\n",command,last_command,strcmp(command, last_command));
            remove(file_path);
            return;
        }
    }
    fprintf(temp, "%s\n", command);
    if(check!=1){
        fprintf(temp,"%s",last_command);
    }
    char buffer[1024];
    int line_count = 1;
    while (fgets(buffer, sizeof(buffer), file) != NULL && line_count < MAX_LOG_SIZE) {
        fputs(buffer, temp);
        line_count++;
    }
    fclose(file);
    fclose(temp);

    remove(cfile_path);
    if (rename(file_path, cfile_path) != 0) {
        perror("rename");
        return;
    }
}

void log_command(int argc, char *argv[]) {
    if (argc == 1) {
        read_log();
    } 
    else if (argc == 2 && strcmp(argv[1], "purge") == 0) {
        purge_log();
    } 
    else if (argc == 3 && strcmp(argv[1], "execute") == 0) {
        int index = atoi(argv[2]);
        execute_log_index(index);
    } 
    else {
        fprintf(stderr, "Invalid log command\n");
    }
}
