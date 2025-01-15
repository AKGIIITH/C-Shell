#ifndef LOG_H
#define LOG_H

void read_log();
void purge_log();
void execute_log_index(int index);
void add_to_log(const char *command);
void log_command(int argc, char *argv[]);


#endif