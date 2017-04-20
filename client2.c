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
} holder;


void error(char *errMsg, int s);


void error(char *errMsg, int s)
{
    if(s < 0)
    {
        perror(errMsg);
        exit(1);
    }
}


int main(int argc, char *argv[1])
{
    int con = 0;
    int length, s, portNum;
    char buffer[BUF];
    struct sockaddr_in serv_addr, cli_addr;


    socklen_t clilen;


    holder dataRec, newData;


    bzero(buffer, BUF);


    if(argc < 2)
    {
        fprintf(stderr, "\nFailed to establish port connection.\n");
        exit(1);
    }


    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    error("You do not have permission to open the socket.\n", s);


    bzero((char *)&serv_addr, sizeof(serv_addr));


    portNum = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portNum);


    if(bind(s, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Socket failed to bind.\n", s);
    }


    clilen = sizeof(cli_addr);


    con = recvfrom(s, &dataRec, sizeof(dataRec), 0, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
    error("Failed to write to socket.", con);


    con = recvfrom(s, buffer, sizeof(buffer), 0, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
    printf("\nData received successfully.\n");


    printf("\nInteger: %d \nCharacter: %c \nFloat: %f\n", dataRec.num, dataRec.letter, dataRec.fNum);
    close(s);
    return 0;
}
