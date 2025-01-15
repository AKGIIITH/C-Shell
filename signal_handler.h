#ifndef SIG_H
#define SIG_H

void handle_ping(char *args[]);
void handle_sigint(int sig);
void handle_sigtstp(int sig);
void handle_sigquit(int sig);
void setup_signal_handlers();

#endif