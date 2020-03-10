#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define THREADS 3
int num_minion=0;
int a[2048];
int arraylen=0, flag=0;
char buffer[2048];
int factor;
int sum[200];
int final_sum=0;
int h=0;

using namespace std;
void* clientThread(void *arg);
void* socketThread(void *arg);

int main(int argc, char *argv[])
{
    struct sockaddr_in GruAddr;
	int socketfd;                              //file descriptor for socket id
	
    if(argc<2)                                 //if portno. is not provided as an argument exit returning (-1)
	{
		fprintf(stderr,"use %s <port>\n",argv[0]);
		exit(-1);                               
	}

	int portno =atoi(argv[1]);
	socketfd = socket(AF_INET,SOCK_STREAM,0);  //using IPv4 connection and TCP protocol
	if(!socketfd)
	{
		perror("Error in opening socket");
		exit(-1);
	}
	GruAddr.sin_family=AF_INET;
	GruAddr.sin_addr.s_addr=htonl(INADDR_ANY);  //specifying IP of the server or use a local address for tranferring of data
	GruAddr.sin_port=htons(portno);

    int check;
    check=bind(socketfd,(struct sockaddr *)&GruAddr,sizeof(GruAddr));
    if(check<0)
	{
		perror("Failed to bind. \n");
		exit(4);
	}

    if(listen(socketfd,50))                                      //specifies the maximum no. of backlog connections
	{
		perror("Error in listening. \n");
		exit(2);
	}

    int optval=1;
	setsockopt(socketfd,SOL_SOCKET,SO_REUSEADDR,(const void *)&optval,sizeof(int));// unknown function
	struct sockaddr_in clientAddr;
	socklen_t cliLen=sizeof(clientAddr);

	pthread_t minions[THREADS];
	pthread_t client;
	
    cliLen= sizeof(clientAddr);
	int newSocketclient=accept(socketfd,(struct sockaddr *)&clientAddr,&cliLen);

    if(newSocketclient<0)
	{
		perror("Error in accepting the connection to a new socket! Exiting (6). \n");
		exit(6);
	}

	if(pthread_create(&client, NULL, clientThread, &newSocketclient) != 0 )
        cout<<"Failed to create thread! "<<endl;

    pthread_join(client,NULL);
    cout<<"The array of numbers received from the Client is: "<<endl;
	int k;
	for(k=0;k<arraylen;k++)
	{
		cout<<a[k]<<" ";
	}
	cout<<""<<endl;
    
    socklen_t minLen; int i=0;
    while(1)
    {
    	minLen= sizeof(clientAddr);
		int newSocket=accept(socketfd,(struct sockaddr *)&clientAddr,&minLen);
		if(newSocket<0)
		{
			perror("Error in accepting connection from new socket!");
			continue;
		}

		num_minion++;
		if(pthread_create(&minions[i++], NULL, socketThread, &newSocket) != 0 )
			cout<<"Failed to create thread"<<endl;
		//pthread_join(minions[i++],NULL);

		if(num_minion==THREADS)
		{
			int v;
			for(v=0;v<num_minion;v++)
			{
				pthread_join(minions[v],NULL);
			}
			
		}

		if(flag==THREADS)
		{
			int u, p;
			for(p=0;p<num_minion;p++)
			{
				final_sum+=sum[p];
			}
			u=send(newSocketclient,&final_sum,sizeof(int),0);
		}
		
		
	}
	
	return 0;
}

void* clientThread(void *arg)
{

  int newSocket = *((int *)arg);
  int i=0;
  cout<<"Client has joined Gru's Server!"<<endl;
  int recvlen;
  recvlen=recv(newSocket,buffer,sizeof(buffer),0);
  buffer[recvlen]='\0';
  // cout<<recvlen<<endl;
  if(recvlen<0);
  {
  	perror("Error in receiving!");
  }
  arraylen=0; int num=0;
  while(buffer[i]!='\0')
  {
  	if(buffer[i]==',')
    {
        i++; 
        a[arraylen]=num;
        arraylen++;
        num=0;
    }
  	else
  	{	
        num=(num*10)+(buffer[i]-'0');
        i++;
    }   

  }
  cout<<"\n"<<arraylen<<"\n"<<endl;
  
}

void* socketThread(void *arg)
{
  int newSocket = *((int *)arg);
  cout<<"Entered into a new thread"<<endl;
  cout<<"Value of num_minion is" <<num_minion<<endl;
  
  int threadid;
  threadid=num_minion-1;
  cout<<"The value of threadid is"<<threadid<<endl;
  sleep(10);
  
  factor=arraylen/THREADS;
  cout<<"The value of factor is "<<factor<<endl;
  
  int m;
  m=send(newSocket,&factor,sizeof(int),0);
  if(m<0)
  {
  	perror("error in sending factor\n");
  }
  int p;
  cout<<"Now sending data"<<endl;
  cout<<"Threadid:"<< threadid << "  Factor value: "<<factor<<endl;
  
  for(p=threadid*factor;p<(threadid+1)*factor;p++)
  {
  	cout<<"\n packet sent"<<endl;
  	int s;
  	s=send(newSocket,&a[p],sizeof(int),0);
  	if(s<0)
  	{
  		perror("error in sending");
  	}
  	
  }
  int f;
  f=recv(newSocket,&sum[threadid],sizeof(int),0);
  cout<<"\n The sum received : "<<sum[threadid]<<endl;
  flag++;
}


