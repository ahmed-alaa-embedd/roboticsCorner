#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket() and bind() */
#include <arpa/inet.h> /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h> /* for atoi() */
#include <string.h>/* for memset() */
#include <unistd.h>/* for close() */

#define ECHOMAX 255 /* Longest string to echo */

void DieWithError(char *errorMessage); /* External error handling function */
int main(int argc, char const *argv[]) 
{ 
    int sock;
    struct sockaddr_in echoServAddr;
    struct sockaddr_in echoClntAddr; /* Client address */
    unsigned int cliAddrLen;  /* Length of incoming message */
    char echoBuffer[ECHOMAX];
    unsigned short echoServPort;
    int recvMsgSize;

    if (argc != 2)
    {
        /* Test for correct number of parameters */
        fprintf(stderr, "Usage: %s <UDP SERVER PORT>\n", argv[0]) ;
        exit(1);
    }

    echoServPort = atoi(argv[1]) ; /* First arg' local port */
    /* Create socket for sending/receiving datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    DieWithError("socket() failed");
    /* Construct local address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family = AF_INET;
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    echoServAddr.sin_port = htons(echoServPort);
    /* Zero out structure */
    /* Internet address family */
    /* Any incoming interface */
    /* Local port */
    /* Bind to the local address */
    if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
    DieWithError("bind() failed");
    for (;;) /* Run forever */
    {
        /* Set the size of the in-out parameter */
        cliAddrLen = sizeof(echoClntAddr);
        /* Block until receive message from a client */
        if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0,
        (struct sockaddr *) &echoClntAddr, &cliAddrLen)) < 0)
        DieWithError("recvfrom() failed") ;
        printf("Handling client %s\n", inet_ntoa(echoClntAddr. sin_addr)) ;

        /* Send received datagram back to the client */
        if (sendto(sock, echoBuffer, recvMsgSize, 0,
        (struct sockaddr *) &echoClntAddr, sizeof(echoClntAddr)) != recvMsgSize)
        DieWithError("sendto() sent a different number of bytes than expected");
    }
    /* NOT REACHED */
    return 0;
}