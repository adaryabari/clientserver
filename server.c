#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#define BUF 80


typedef struct data
{
    int num;
    char letter;
    float fNum;
} holder;


holder modify(holder h);
void error(char *errMsg, int s);


void error(char *errMsg, int s)
{
    if (s < 0)
    {
        perror(errMsg);
        exit(1);
    }
}


holder modify(holder h)
{
    h.num = (h.num) * 2;
    h.fNum = (h.fNum) + 1;
    if(h.letter == 122)
    {
        h.letter = 97;
    }
    else if(h.letter == 90)
    {
        h.letter = 65;
    }
    else
    {
        h.letter = h.letter + 1;
    }
    return h;
}


int main(int argc, char *argv[])
{


    int con = 0;
    int length, portNum, pChar, s, s2;
    char buffer[BUF];
    struct sockaddr_in serv_addr, cli_addr, cli2_addr;
    struct hostent *server;
    socklen_t clilen;
    holder dataRec, newData;


    bzero(buffer, BUF);


    if (argc < 2)
    {
        fprintf(stderr, "Failed to establish a connection.\n"); // Checks for port
        exit(1);
    }


    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // Opens a socket
    error("Failed to open socket.", s);


    s2 = socket(AF_INET, SOCK_DGRAM, 0); // Opens a new socket
    error("Failed to open socket.", s2);


    portNum = atoi(argv[1]); // Ascii to Integer
    server = gethostbyname(argv[2]); // Server connected
    pChar =  atoi(argv[3]); //Ascii  to Integer


    bzero((char *) &serv_addr, sizeof(serv_addr));
    bzero((char *) &cli2_addr, sizeof(cli2_addr));


    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portNum);


    cli2_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&cli2_addr.sin_addr.s_addr, server->h_length); // moves bytes from pointer 1 to pointer 2
    cli2_addr.sin_port = htons(pChar);
    length = sizeof(struct sockaddr_in);


    if (bind(s, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Failed to bind socket.", s);
    }


    if (bind(s2, (struct sockaddr *) &cli2_addr, sizeof(cli2_addr)) < 0)
    {
        error("Failed to bind socket.", s2);
    }


    clilen = sizeof(cli_addr);
    con = recvfrom(s, &dataRec, sizeof(dataRec), 0, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);


    error("Failed to write to socket.", con);


    con = recvfrom(s, buffer, sizeof(buffer), 0, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
    sscanf(buffer, "%f", &dataRec.fNum);


    printf("\nData received successfully.\n\n");
		printf("***Original Data***\n");
    printf("\nInteger: %d \nCharacter: %c \nFloat: %f\n", dataRec.num, dataRec.letter, dataRec.fNum);


    newData = modify(dataRec);


    printf("***Modified Data***\n");
    printf("\nInteger: %d \nCharacter: %c \nFloat: %f\n", newData.num, newData.letter, newData.fNum);




    con = sendto(s,"Data received successfully.\n",26, 0,(struct sockaddr *)&cli_addr,(socklen_t)clilen); //write()
    con = sendto(s2, &newData, sizeof(newData), 0, (struct sockaddr *)&cli2_addr, length);
    error("Failed to write to socket.", con);
    con = sendto(s2, buffer, sizeof(buffer), 0, (struct sockaddr *)&cli2_addr, length);
    error("Failed to write to socket.", con);


    printf("\nData sent to server successfully. <%s>",argv[2]);
    printf("\nConnection ending.\n");
    close(s); // Disconnects
    close(s2);
    return 0;
}
