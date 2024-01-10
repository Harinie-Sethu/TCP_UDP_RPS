#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 8080
#define BUFFER_SIZE 1024

// Function to determine the game result
// Returns: 0 for Draw, 1 for Player A wins, 2 for Player B wins
int getGameResult(int choiceA, int choiceB)
{
    if (choiceA == choiceB)
    {
        return 0; // Draw
    }
    else if ((choiceA == 0 && choiceB == 2) || (choiceA == 1 && choiceB == 0) || (choiceA == 2 && choiceB == 1))
    {
        return 1; // Player A wins
    }
    else
    {
        return 2; // Player B wins
    }
}

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
        int choiceA, choiceB;

        // Receive data from client A
        int bytes_received_A = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);
        if (bytes_received_A == -1)
        {
            perror("Receive error");
            close(sockfd);
            exit(1);
        }

        buffer[bytes_received_A] = '\0';
        choiceA = atoi(buffer);

        // Receive data from client B
        int bytes_received_B = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);
        if (bytes_received_B == -1)
        {
            perror("Receive error");
            close(sockfd);
            exit(1);
        }

        buffer[bytes_received_B] = '\0';
        choiceB = atoi(buffer);

        // Determine the game result
        int gameResult = getGameResult(choiceA, choiceB);
        const char *resultMessage;

        if (gameResult == 0)
        {
            resultMessage = "Draw";
        }
        else if (gameResult == 1)
        {
            resultMessage = "Client A Wins";
        }
        else
        {
            resultMessage = "Client B Wins";
        }

        // Send the result to both clients
        if (sendto(sockfd, resultMessage, strlen(resultMessage), 0, (struct sockaddr *)&client_addr, client_len) == -1)
        {
            perror("Sendto error");
            close(sockfd);
            exit(1);
        }

        // Prompt clients for another game
        char playAgainBuffer[BUFFER_SIZE];
        const char *playAgainMessage = "Do you want to play another game? (yes/no):  ";
        if (sendto(sockfd, playAgainMessage, strlen(playAgainMessage), 0, (struct sockaddr *)&client_addr, client_len) == -1)
        {
            perror("Sendto error");
            close(sockfd);
            exit(1);
        }

        // Receive play again response from both clients
        int bytes_received_playAgain_A = recvfrom(sockfd, playAgainBuffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);
        int bytes_received_playAgain_B = recvfrom(sockfd, playAgainBuffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);

        playAgainBuffer[bytes_received_playAgain_A] = '\0';

        if (bytes_received_playAgain_A == -1 || bytes_received_playAgain_B == -1)
        {
            perror("Receive error");
            close(sockfd);
            exit(1);
        }

        if (strcmp(playAgainBuffer, "yes") != 0)
        {
            // If any client says "no," exit the server
            break;
        }
    }

    close(sockfd);
    return 0;
}
