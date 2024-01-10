#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int sockfd;
    struct sockaddr_in server_addr;
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
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Send data to server
    char message[] = "first msg - Hello, server!";
    if (sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Sendto error");
        close(sockfd);
        exit(1);
    }

    printf("Message sent to server: %s\n", message);

    // Receive response from server
    int bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);
    if (bytes_received == -1)
    {
        perror("Receive error");
        close(sockfd);
        exit(1);
    }

    buffer[bytes_received] = '\0';
    printf("Received from server: %s\n", buffer);

    close(sockfd);
    return 0;
}
