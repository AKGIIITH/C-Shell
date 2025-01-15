#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include "signal_handler.h"
#include "global.h"

// Function to handle the `ping` command
void handle_ping(char *args[]) {
    if (args[1] == NULL || args[2] == NULL) {
        fprintf(stderr, "Usage: ping <pid> <signal_number>\n");
        return;
    }

    pid_t pid = atoi(args[1]);
    int signal_number = atoi(args[2]);

    // Normalize the signal number
    int sig_num = signal_number % 32;
    if (sig_num < 1 || sig_num > 31) {
        fprintf(stderr, "Invalid signal number\n");
        return;
    }

    // Check if the signal number corresponds to a valid signal
    if (sig_num == SIGKILL) {
        printf("Sent signal %d to process with PID %d\n", sig_num, pid);
        if (kill(pid, SIGKILL) == -1) {
            if (errno == ESRCH) {
                printf("No such process found\n");
            } else {
                perror("Error sending signal");
            }
        }
    } else if (sig_num == SIGTERM) {
        printf("Sent signal %d to process with PID %d\n", sig_num, pid);
        if (kill(pid, SIGTERM) == -1) {
            if (errno == ESRCH) {
                printf("No such process found\n");
            } else {
                perror("Error sending signal");
            }
        }
    } else {
        // Handle other signals
        printf("Sent signal %d to process with PID %d\n", sig_num, pid);
        if (kill(pid, sig_num) == -1) {
            if (errno == ESRCH) {
                printf("No such process found\n");
            } else {
                perror("Error sending signal");
            }
        }
    }
}


// Global variable to track the foreground process PID
pid_t fg_pid = -1;

// Signal handler for SIGINT (Ctrl-C)
void handle_sigint(int sig) {
    if (fg_pid != -1) {
        printf("Sending SIGINT to PID %d\n", fg_pid);
        if (kill(fg_pid, SIGINT) == -1) {
            perror("Error sending SIGINT");
        }
    }
}

// Signal handler for SIGTSTP (Ctrl-Z)
void handle_sigtstp(int sig) {
    if (fg_pid != -1) {
        printf("Sending SIGTSTP to PID %d\n", fg_pid);
        if (kill(fg_pid, SIGTSTP) == -1) {
            perror("Error sending SIGTSTP");
        }
    }
}

// Signal handler for SIGQUIT (Ctrl-D)
void handle_sigquit(int sig) {
    printf("Logging out\n");
    // Kill all processes and exit
    exit(EXIT_SUCCESS);
}

// Function to initialize signal handlers
void setup_signal_handlers() {
    struct sigaction sa;
    
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    sa.sa_handler = handle_sigtstp;
    if (sigaction(SIGTSTP, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    sa.sa_handler = handle_sigquit;
    if (sigaction(SIGQUIT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
}


