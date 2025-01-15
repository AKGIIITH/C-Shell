#include "iMan.h"

void iMan(const char *command) {
    struct sockaddr_in server_addr;
    struct hostent *server;
    int sockfd, bytes_read;
    char buffer[BUFFER_SIZE];
    char request[BUFFER_SIZE];

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(1);
    }

    // Get the server's DNS entry
    server = gethostbyname(HOST);
    if (server == NULL) {
        fprintf(stderr, "Error: No such host\n");
        exit(1);
    }

    // Set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error connecting to server");
        exit(1);
    }

    // Formulate the GET request
    snprintf(request, sizeof(request),
             "GET /man/%s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "Connection: close\r\n\r\n",
             command, HOST);

    // Send the request
    if (send(sockfd, request, strlen(request), 0) < 0) {
        perror("Error sending request");
        exit(1);
    }

    // Read the response and print it to stdout
    int header_end = 0;
    while ((bytes_read = read(sockfd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_read] = '\0';

        // Ignore headers by detecting the end of headers (CRLFCRLF)
        if (!header_end) {
            char *body = strstr(buffer, "\r\n\r\n");
            if (body) {
                header_end = 1;
                printf("%s", body + 4);  // Skip the headers and print the body
            }
        } else {
            printf("%s", buffer);
        }
    }

    // Close the socket
    close(sockfd);
}