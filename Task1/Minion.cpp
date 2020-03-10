#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>

using namespace std;

int main(int argc,char *argv[])
{
    struct sockaddr_in GruAddr;
    struct hostent *Gru;
    char *hostname;
    int socketfd;
    if(argc <3)
    {
        fprintf(stderr,"Provide arguments as: %s <hostname> <port>\n Exiting(-1).",argv[0]);
        exit(-1);
    }
    hostname=argv[1];
    int portno =atoi(argv[2]);

    socketfd=socket(AF_INET,SOCK_STREAM,0);
    if(!socketfd)
    {
        perror("Error in opening the socket");
        exit(-1);
    }
    Gru = gethostbyname(hostname);
    if(Gru==NULL)
    {
        fprintf(stderr,"Can't resolve hostname. Exiting (2). \n");
        exit(2);
    }
    bzero((char *)&GruAddr,sizeof(GruAddr));
    GruAddr.sin_family=AF_INET;
    bcopy((char *)Gru->h_addr,(char *)&GruAddr.sin_addr.s_addr,Gru->h_length);
    GruAddr.sin_port=htons(portno);

    if(connect(socketfd,(struct sockaddr *)&GruAddr,sizeof(GruAddr))<0)
    {
        perror("connect");
        exit(3);
    }

    char buf[] ="Hello there, Gru! A Minion ready to join you.";
    int bufLen=strlen(buf);
    int subarraylen=0;
    int packet=0;
    packet=recv(socketfd,&subarraylen,4,0);

    if(packet<0)
    {
        perror("Error in revieving subarraylength. \n");
    }
    cout << "\nThe subarraylen is "<<subarraylen<<endl;
    int a[subarraylen];
    int serLen= sizeof(GruAddr);
    int recvLen=0;
    for(int z=0;z<subarraylen;z++)
    {
        recvLen=0;
        recvLen=recv(socketfd,&a[z],4,0);
        if(recvLen<0)
        {
            perror("Error in recieving.");
            exit(5);
        }

    }
    
    cout<<"The subarray received is : "<< endl;
    for(int p=0;p<subarraylen;p++)
        cout<<a[p]<<" ";

    int sum=0;
    for(int i=0;i<subarraylen;i++)
    {
        sum=sum+a[i];
    }
    cout<<"The sum of subarray is :"<<sum<<endl;
    int m;
    m=send(socketfd,&sum,sizeof(int),0);
    if(m<0)
    {
        perror("Error in sending sum to Gru.\n");
    }
    close(socketfd);
    return 0;
}