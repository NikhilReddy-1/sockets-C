//
// Created by nikhil on 11/06/20.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>
#include <strings.h>

int main(int argc, char *argv[])
{
    int clientSock,port;
    struct sockaddr_in clientAddr;
    int addrLen = sizeof(clientAddr);
    char buffer[1024];

    if (argc < 2){
        perror("Too few arguments");
        exit(1);
    }

    port = atoi(argv[1]);


    clientSock = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSock == -1){
        perror("Client socket failure!\n");
    }

    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = INADDR_ANY;
    clientAddr.sin_port = htons(port);

    if ((connect(clientSock,(struct sockaddr_in*)&clientAddr,(socklen_t*)&addrLen)) == -1){
        perror("Connection failure!\n");
        exit(1);
    }

    while (1)
    {
        bzero(buffer,1024);
        fgets(buffer,1024,stdin);

        if ((write(clientSock,buffer,1024)) == -1){
            perror("Write error!\n");
            exit(1);
        }

        bzero(buffer,1024);

        if ((read(clientSock,buffer,1024)) == -1){
            perror("Read error!\n");
            exit(1);
        }
        printf("Server : %s\n",buffer);

        int i = strncmp("bye",buffer,3);
        if(i == 0)
            break;

    }

    close(clientSock);
    return 0;

}
