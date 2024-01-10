#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SERVER_IP "127.0.0.1"
#define PORT_A 5566
#define PORT_B 5567

int main()
{
    int sock;
    char bufferSpace[1024];
    struct sockaddr_in serverAddr;
    socklen_t addrSize = sizeof(serverAddr);

    // Create TCP socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }

    // Initialize server address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;

    // Connect to the server based on which port this client is for
    int port;
    // if (/* This is Client A */) {
    //     port = PORT_A;
    // } else {
    //     port = PORT_B;
    // }
    port = PORT_A;

    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Send connection request to the server
    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("Connect error");
        exit(1);
    }

    printf("Connected to the server.\n");

    while (1)
    {

        // Implement user input and game logic here
        char userChoice[10];
        char playAgain[10];

        printf("Enter your choice - Rock (r) or Paper (p) or Scissors (s): ");
        scanf("%s", userChoice);
        send(sock, userChoice, strlen(userChoice), 0);

        // Receive the result from the server
        bzero(bufferSpace, sizeof(bufferSpace));
        recv(sock, bufferSpace, sizeof(bufferSpace), 0);
        printf("Server: %s\n", bufferSpace);

        // Prompt for another game or exit
        printf("Do you want to play another game? (yes/no): ");
        scanf("%s", playAgain);
        send(sock, playAgain, strlen(playAgain), 0);

        if (strcmp(playAgain, "no") == 0)
        {
            break; // Exit the loop if the user chooses not to play again
        }
    }

    // Close the connection
    close(sock);
    printf("Disconnected from the server.\n");

    return 0;
}
