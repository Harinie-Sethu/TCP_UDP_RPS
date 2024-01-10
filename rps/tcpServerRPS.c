#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>

#define PORT_A 5566
#define PORT_B 5567
#define SERVER_IP "127.0.0.1"  // Change this to the server's IP address


int main()
{
    int serverSockA, serverSockB, clientSockA = -1, clientSockB = -1;
    struct sockaddr_in serverAddrA, serverAddrB, clientAddr;
    socklen_t addrSize = sizeof(clientAddr);
    fd_set readfds;

    // Create TCP sockets for clients A and B
    serverSockA = socket(AF_INET, SOCK_STREAM, 0);
    serverSockB = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSockA < 0 || serverSockB < 0)
    {
        perror("Creating Socket error");
        exit(1);
    }

    // Initialize server addresses for clients A and B
    memset(&serverAddrA, 0, sizeof(serverAddrA));
    serverAddrA.sin_family = AF_INET;
    serverAddrA.sin_port = htons(PORT_A);
    serverAddrA.sin_addr.s_addr = INADDR_ANY;

    memset(&serverAddrB, 0, sizeof(serverAddrB));
    serverAddrB.sin_family = AF_INET;
    serverAddrB.sin_port = htons(PORT_B);
    serverAddrB.sin_addr.s_addr = INADDR_ANY;

    // Bind sockets to respective ports
    if (bind(serverSockA, (struct sockaddr *)&serverAddrA, sizeof(serverAddrA)) < 0 ||
        bind(serverSockB, (struct sockaddr *)&serverAddrB, sizeof(serverAddrB)) < 0)
    {
        perror("Binding socket error");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(serverSockA, 1) == -1 || listen(serverSockB, 1) == -1)
    {
        perror("Error listening");
        close(serverSockA);
        close(serverSockB);
        exit(1);
    }

    printf("Server is ready. Waiting for clients A and B...\n");

    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(serverSockA, &readfds);
        FD_SET(serverSockB, &readfds);

        // Use select to monitor multiple file descriptors
        int maxSock = (serverSockA > serverSockB) ? serverSockA : serverSockB;
        int activity = select(maxSock + 1, &readfds, NULL, NULL, NULL);

        if (activity < 0)
        {
            perror("Select error");
            exit(1);
        }

        // Handle incoming connections from clients A and B
        if (FD_ISSET(serverSockA, &readfds))
        {
            if (clientSockA == -1)
            {
                clientSockA = accept(serverSockA, (struct sockaddr *)&clientAddr, &addrSize);
                printf("Client A connected.\n");
            }
        }

        if (FD_ISSET(serverSockB, &readfds))
        {
            if (clientSockB == -1)
            {
                clientSockB = accept(serverSockB, (struct sockaddr *)&clientAddr, &addrSize);
                printf("Client B connected.\n");
            }
        }

        // Handle game logic if both clients are connected
        if (clientSockA != -1 && clientSockB != -1)
        {
            char result[50];
            char choiceA[10], choiceB[10];

            // Receive choices from clients
            recv(clientSockA, choiceA, sizeof(choiceA), 0);
            recv(clientSockB, choiceB, sizeof(choiceB), 0);

            // Implement the Rock, Paper, Scissors game logic here
            // Replace this with your own game logic
            if (strcmp(choiceA, choiceB) == 0)
            {
                sprintf(result, "Draw");
            }
            else if ((strcmp(choiceA, "r") == 0 && strcmp(choiceB, "s") == 0) ||
                     (strcmp(choiceA, "p") == 0 && strcmp(choiceB, "r") == 0) ||
                     (strcmp(choiceA, "s") == 0 && strcmp(choiceB, "p") == 0))
            {
                sprintf(result, "Client A Wins");
            }
            else
            {
                sprintf(result, "Client B Wins");
            }

            // Send the result to both clients
            send(clientSockA, result, strlen(result), 0);
            send(clientSockB, result, strlen(result), 0);

            // Prompt clients for another game
            char playAgain[10];
            recv(clientSockA, playAgain, sizeof(playAgain), 0);
            recv(clientSockB, playAgain, sizeof(playAgain), 0);

            if (strcmp(playAgain, "no") == 0)
            {
                break; // Exit the loop if any client chooses not to play again
            }
        }
    }

    close(clientSockA);
    close(clientSockB);
    close(serverSockA);
    close(serverSockB);

    return 0;
}
