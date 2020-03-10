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
    struct hostent *Gru; char *hostname;
    
    int socketfd;
    if(argc <3)
    {
        fprintf(stderr,"Provide arguments as:  %s <hostname> <port>\n",argv[0]);
        exit(-1);
    }

    hostname=argv[1];
    int portno =atoi(argv[2]);
    char buffer[2048];
    cout<<"Enter numbers with (,) between them: "<<endl;
    cin>>buffer;
    int bufferlen=strlen(buffer);
    
    /*int a[6];
    for(int q=0;q<6;q++)
    {
        printf("Enter the element %d of array\n",q+1);
        scanf("%d",&a[q]);
    }*/
    
    
    socketfd=socket(AF_INET,SOCK_STREAM,0);
    if(!socketfd)
    {
        perror("Error in opening socket!! Exiting (-1).");
        exit(-1);
    }
    Gru = gethostbyname(hostname);
    if(Gru==NULL)
    {
        fprintf(stderr,"Can't resolve hostname!! Exiting (2). \n");
        exit(2);
    }

    bzero((char *)&GruAddr,sizeof(GruAddr));
    GruAddr.sin_family=AF_INET;
    bcopy((char *)Gru->h_addr,(char *)&GruAddr.sin_addr.s_addr,Gru->h_length);
    GruAddr.sin_port=htons(portno);

    if(connect(socketfd,(struct sockaddr *)&GruAddr,sizeof(GruAddr))<0)
    {
        perror("Error in connecting to the main server(Gru). Exiting (3).");
        exit(3);
    }
    
    int serLen= sizeof(GruAddr);
    int recvLen=0;
    if(send(socketfd,buffer,bufferlen,0)<0)//data being sent to client side
    {
        perror("Error in sending to Gru.");
        exit(5);
    }
    int final_sum;
    recvLen=recv(socketfd,&final_sum,sizeof(int),0);
    cout<<"Final sum received is : "<<final_sum;
    close(socketfd);
    return 0;
}