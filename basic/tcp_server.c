#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h> /* Internet domain header */
#include <arpa/inet.h>  /* only needed on my mac */
#include <netdb.h>      /* gethostname */

int main()
{
    // IP (Internet Protocol) address and port number - used to make socket
    char *ip = "127.0.0.1";
    int port = 5566;

    int serverSock, clientSock, n;
    char bufferSpace[1024];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    // create TCP socket returning a socket descriptor
    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0)
    {
        perror("Creating Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    // initialize server address
    // provide ip and port
    // address(Server and client) stored in sockaddr_in struct

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    // binding socket descriptor with server address

    n = bind(serverSock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (n < 0)
    {
        perror("Binding scoket error");
        exit(1);
    }
    printf("[+]Bind to the port number: %d\n", port);

    // listen for incomming connections
    if (listen(serverSock, 5) == -1)
    {
        perror("Error listening");
        close(serverSock);
        exit(1);
    }
    printf("Listening...\n");

    // server can only handle one client at a time
    // while (1)
    // {
    // accept client connection
    addr_size = sizeof(client_addr);
    clientSock = accept(serverSock, (struct sockaddr *)&client_addr, &addr_size);
    printf("[+]Client connected.\n");

    // recieve message from client
    bzero(bufferSpace, 1024);
    recv(clientSock, bufferSpace, sizeof(bufferSpace), 0);
    printf("Client: %s\n", bufferSpace);

    // server sends reply
    bzero(bufferSpace, 1024);
    strcpy(bufferSpace, "HI, THIS IS SERVER. What's the best thing about Switzerland?");
    printf("Server: %s\n", bufferSpace);
    send(clientSock, bufferSpace, strlen(bufferSpace), 0);

    // close client connection
    close(clientSock);
    printf("[+]Client disconnected.\n\n");
    // }

    return 0;
}