#ifndef IMAN_H
#define IMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

// Buffer size for reading data from the socket
#define BUFFER_SIZE 4096
// Host name for fetching the man page
#define HOST "man.he.net"
// Port number for HTTP connection
#define PORT 80

// Function to fetch and display the man page for a given command
void iMan(const char *command);

#endif
