#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>


int main(int argc, char *argv[])
{
    int serversock,port,bb,new_sock;
    struct sockaddr_in serverAddr;
    char buffer[1024];
    int addrLen = sizeof(serverAddr);


    if (argc < 2)
    {
        printf("Too few Arguments!\n");
        exit(1);
    }

    port = atoi(argv[1]);

    serversock = socket(AF_INET,SOCK_STREAM,0);

    if (serversock == -1){
        perror("Socket Set up failure!\n");
        exit(1);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    bb = bind(serversock, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (bb < 0){
        perror("Binding error\n");
        exit(1);
    }

    if (listen(serversock, 5) == -1){
        perror("Listen Failure!\n");
        exit(1);
    }

    if((new_sock = accept(serversock,(struct sockaddr*)&serverAddr,(socklen_t*)&addrLen)) == -1){
        perror("Accept failure!\n");
        exit(1);
    }

    while (1)
    {
        bzero(buffer,1024);

        if((read(new_sock,buffer,1024)) == -1){
            perror("Read error!\n");
        }
        printf("Client : %s\n",buffer);
        bzero(buffer,1024);

        fgets(buffer,1024,stdin);

        if((write(new_sock,buffer,1024)) == -1){
            perror("Write failure!\n");
            exit(1);
        }

        int i = strncmp("bye",buffer,3);
        if(i == 0)
            break;
    }

    close(new_sock);
    close(serversock);
    return 0;



}
