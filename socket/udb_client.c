/* 
we must enter 2 arguments  
<Server IP> 
<Echo Word> the word we need to echo
the 3rd arguments is port 7 if not entered 
notice we need to communicate to the same port
*/

#include <stdio.h>/* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <arpa/inet.h> /* for sockaddr_in and inet_addr() */
#include <stdlib.h> /* for atoi() */
#include <string.h>/* for memset() */
#include <unistd.h>/* for close() */

#define ECHOMAX 255/* Longest string to echo */

void DieWithError(char *errorMessage); /* External error handling function */

int main(int argc, char const *argv[])
{
    int sock;/* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    struct sockaddr_in fromAddr;/* Source address of echo */
    unsigned short echoServPort;/* Echo server port */
    unsigned int fromSize;/* In-out of address size for recvfrom() */
    char *servlP;/* IP address of server */
    char *echoString;/* String to send to echo server */
    char echoBuffer[ECHOMAX+1];/* Buffer for receiving echoed string */
    int echoStringLen;/* Length of string to echo */
    int respStringLen;/* Length of received response */

    if ((argc < 3) || (argc> 4))/* Test for correct number of arguments */
    {
        fprintf(stderr,"Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n", argv[0]);
        exit(1);
    }
    servlP = argv[1] ;/* First arg' server IP address (dotted quad)*/
    echoString = argv[2] ; /* Second arg' string to echo */
    /* Check input length */
    if ((echoStringLen = strlen(echoString)) > ECHOMAX)
        DieWithError("Echo word too long");

    if (argc == 4)
        echoServPort = atoi(argv[3]) ; /* Use given port, if any */
    else
        echoServPort = 7; /* 7 is the well-known port for the echo service */

    /* Create a datagram/UDP socket */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        DieWithError( "socket () failed") ;

    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));/* Zero out structure */
    echoServAddr.sin_family = AF_INET;/* Internet addr family */
    echoServAddr.sin_addr.s_addr = inet_addr(servlP);/* Server IP address */
    echoServAddr.sin_port= htons(echoServPort);/* Server port */

    /* Send the string to the server */
    if (sendto(sock, echoString, echoStringLen, 0, (struct sockaddr *)
    &echoServAddr, sizeof(echoServAddr)) != echoStringLen)
        DieWithError("sendto() sent a different number of bytes than expected");

    /* Recv a response */
    fromSize = sizeof(fromAddr) ;

    if ((respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0,
    (struct sockaddr *) &fromAddr, &fromSize)) != echoStringLen)
        DieWithError("recvfrom() failed") ;

    if (echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr)
    {
        fprintf(stderr,"Error: received a packet from unknown source.\n");
        exit(1);
    }

    /* null-terminate the received data */
    echoBuffer[respStringLen] = '\0' ;
    printf("Received: %s\n", echoBuffer);
    /* Print the echoed arg */
    close(sock);
    exit(0);
}
