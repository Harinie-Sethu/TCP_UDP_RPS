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

    while (1)
    {
        int choice;

        // Prompt the user to enter their choice
        printf("Enter your choice - Rock (0) or Paper (1) or Scissors (2):");
        scanf("%d", &choice);

        // Send the choice to the server
        if (sendto(sockfd, &choice, sizeof(int), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        {
            perror("Sendto error");
            close(sockfd);
            exit(1);
        }

        // Receive and display the game result
        int bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);
        if (bytes_received == -1)
        {
            perror("Receive error");
            close(sockfd);
            exit(1);
        }

        buffer[bytes_received] = '\0';
        printf("Result: %s\n", buffer);

        // Prompt for another game
        char playAgainResponse[BUFFER_SIZE];
        printf("Do you want to play another game? (yes/no): ");
        scanf("%s", playAgainResponse);

        // Send play-again response to the server
        if (sendto(sockfd, playAgainResponse, strlen(playAgainResponse), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
        {
            perror("Sendto error");
            close(sockfd);
            exit(1);
        }

        if (strcmp(playAgainResponse, "no") == 0)
        {
            // Exit the client if the user chooses not to play again
            break;
        }
    }

    close(sockfd);
    printf("Disconnected from the server.\n");

    return 0;
}
