#include <arpa/inet.h>
#include<netinet/in.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include<string.h>
#include <netdb.h>
#include<iostream>
#include<fstream>
using namespace std;
int main(int argc,char *argv[])
{
    struct sockaddr_in serverAddr;
    struct hostent *server;
    char *hostname;
    int socketfd;
    if(argc <3)
    {
        fprintf(stderr,"use %s <hostname> <port>\n",argv[0]);
        exit(-1);
    }
    hostname=argv[1];
    int portno =atoi(argv[2]);
    socketfd=socket(AF_INET,SOCK_STREAM,0);
    if(!socketfd)
    {
        perror("Error in opening socket");
        exit(-1);
    }
    server = gethostbyname(hostname);
    if(server==NULL)
    {
        fprintf(stderr,"can't resolve hostname \n");
        exit(2);
    }
    bzero((char *)&serverAddr,sizeof(serverAddr));
    serverAddr.sin_family=AF_INET;
    bcopy((char *)server->h_addr,(char *)&serverAddr.sin_addr.s_addr,server->h_length);
    serverAddr.sin_port=htons(portno);
    if(connect(socketfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr))<0)
    {
        perror("connect");
        exit(3);
    }
    
    int serLen= sizeof(serverAddr);
    char buffer[50];

    int bufferlen=sizeof(buffer);
    while(1)
    {
        cout<<"Enter data in the format E.g. 1 U 6 "<<endl;
        cin.getline(buffer,sizeof(buffer));
        cout<<buffer<<endl;
        if(send(socketfd,buffer,bufferlen,0)<0)//data being sent to client side
        {
            perror("error in sending");
            exit(5);
        }

    }
    close(socketfd);
    return 0;
}