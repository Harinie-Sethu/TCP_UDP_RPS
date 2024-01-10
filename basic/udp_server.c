#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("Socket creation error");
        exit(1);
    }

    // Initialize server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to server address
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Binding error");
        close(sockfd);
        exit(1);
    }

    printf("UDP server is waiting for messages...\n");

    while (1)
    {
        // Receive data from client
        int bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);
        if (bytes_received == -1)
        {
            perror("Receive error");
            close(sockfd);
            exit(1);
        }

        buffer[bytes_received] = '\0';
        printf("Received from client: %s\n", buffer);

        // Send a response back to the client
        char response[] = "response to first msg - Hello back to you!";
        if (sendto(sockfd, response, strlen(response), 0, (struct sockaddr *)&client_addr, client_len) == -1)
        {
            perror("Sendto error");
            close(sockfd);
            exit(1);
        }
    }

    close(sockfd);
    return 0;
}
