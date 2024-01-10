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

    int sock, n;
    char bufferSpace[1024];
    struct sockaddr_in addr;
    socklen_t addr_size;

    // create tcp socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    // provide ip and port
    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr.s_addr = inet_addr(ip);

    // send connection request to server
    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    printf("Connected to the server.\n");

    // send message to server and wait for reply
    bzero(bufferSpace, 1024);
    strcpy(bufferSpace, "HELLO, THIS IS CLIENT. TELL ME A JOKE");
    printf("Client: %s\n", bufferSpace);
    send(sock, bufferSpace, strlen(bufferSpace), 0);

    // get reply from server
    bzero(bufferSpace, 1024);
    recv(sock, bufferSpace, sizeof(bufferSpace), 0);
    printf("Server: %s\n", bufferSpace);

    // close connection
    close(sock);
    printf("Disconnected from the server.\n");

    return 0;

}