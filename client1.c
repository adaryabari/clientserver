#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#define BUF 80

typedef struct data
{
    int num;
    char letter;
    float fNum;
} data;

void error(char *errMsg, int s);

void error(char *errMsg, int s)
{
    if(s < 0)
    {
        perror(errMsg);
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    int s, length, con, portNum;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[BUF];
    data dataSend, dataBuffer;

    if (argc < 3)
    {
        fprintf(stderr, "Usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portNum = atoi(argv[2]); // Ascii -> Integer
    s = socket(AF_INET, SOCK_DGRAM, 0); //opens new socket
    error("Failed to open socket.", s);

    server = gethostbyname(argv[1]); // Connect to Server

    if (server == NULL)
    {
        fprintf(stderr, "Failed to connect to host.\n"); //Checks for server
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr)); //clears pointer
    serv_addr.sin_family = AF_INET;

    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length); //moves bytes from s1 to s2

    serv_addr.sin_port = htons(portNum);

    length = sizeof(struct sockaddr_in);

    printf("\nServer connection completed successfully. <%s>\n", argv[1]);
    printf("\nEnter an integer: ");
    scanf("%d", &dataBuffer.num);
    printf("\nEnter a character: ");
    scanf("\n%c", &dataBuffer.letter);
    printf("\nEnter a decimal number: ");
    scanf("%f", &dataBuffer.fNum);

    dataSend.num = htonl((uint32_t)dataBuffer.num); // Receive from Client1
    dataSend.letter = dataBuffer.letter;
    dataSend.fNum = dataBuffer.fNum;

    bzero(buffer, BUF);
    sprintf(buffer,"%f", dataSend.fNum);

    con = sendto(s, &dataSend, sizeof(dataSend), 0, (struct sockaddr *)&serv_addr, length);
    error("Data transmission failed.", con);

    con = sendto(s, buffer, sizeof(buffer), 0, (struct sockaddr *)&serv_addr, length);
    error("Server communication failed.", con);

    con = recvfrom(s, buffer, sizeof(buffer), 0, (struct sockaddr *)&serv_addr, (socklen_t *)&length);
    error("No response from server.", con);

    printf("\nData sent to server successfully. <%s>", argv[1]);
    printf("\nMessage received from server: <%s>\n", buffer);
    printf("\nEnding connection.\n");
    close(s); // Disconnect

    return 0;
}
