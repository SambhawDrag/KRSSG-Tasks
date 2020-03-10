#include <iostream>
#include <time.h>
#include <math.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX 20
#define TIME 1
char buffer[50];   //buffer for accept the message from the client

using namespace std;

int enter,out;   //Global Variables
char trans;

void input(int *arg)    //Input function to accept the data from the Client
{
  int newsocket=*arg;
  int recvlen;
  recvlen=recv(newsocket,buffer,sizeof(buffer),0);
  cout<<"recvlen = "<<recvlen<<endl;
  buffer[recvlen]='\0';

  if(recvlen<0);
  {
    perror("error in receiving");
  }
  enter=buffer[0]-'0';
  trans=buffer[1];
  out=buffer[2]-'0';
}

int nearest(int enter,int pos_lift1,int pos_lift2)
{
    if(abs(pos_lift1-enter)<abs(pos_lift2-enter))
        return 1;
    else
        return 2;
}

class Lift {             //Definition of Class Lift
public:
    char state;
    int current_pos, initial_pos, next_stop;
    int in[MAX], out[MAX];
    char tr[MAX];
};

class System {           //Definition of Class System
public:
    Lift lift1, lift2;

    //System Properties:
    int max_floor, min_floor;

    //Transition functions:-
        //Transition functions:-
    void transitionup(int* curpos, int enter, char tr, int out, int num)
    {
        int* p = curpos;
        int i;

        if (*curpos <= enter)
        {
            for (i = *curpos; i < enter; i++)
            {
                sleep(1);
                cout<<"Lift "<<num<<"at floor: "<<*p<<"State: MOVING UP"<<endl;
                (*p)++;
            }
        }
        else
        {
            for (i = *curpos; i > enter; i--)
            {
                sleep(1);
                cout<<"Lift "<<num<<"at floor: "<<*p<<"State: MOVING DOWN"<<endl;
                (*p)--;
            }
        }

        *curpos = enter;
        cout<<"Lift "<<num<<"at floor: "<<enter<<"State: REST"<<endl;

        for (i = enter + 1; i < out; i++)
        {
            sleep(1);
            cout<<"Lift "<<num<<"at floor: "<<*p<<"State: MOVING UP"<<endl;
            (*p)++;
        }
        *(curpos) = out;
        *p = out;

    }

    void transitiondown(int* curpos, int enter, char tr, int out, int num)
    {
        int* p = curpos;
        int i;

        if (*curpos > enter)
        {
            for (i = *curpos; i > enter; i--)
            {
                sleep(1);
                cout<<"Lift "<<num<<"at floor: "<<*p<<"State: MOVING DOWN"<<endl;
                (*p)--;
            }
        }
        else
        {
            for (i = *curpos; i < enter; i++)
            {
                sleep(1);
                cout<<"Lift "<<num<<"at floor: "<<*p<<"State: MOVING UP"<<endl;
                (*p)++;
            }
        }
        *curpos = enter;

        for (i = enter - 1; i > out; i--)
        {
            sleep(1);
            cout<<"Lift "<<num<<"at floor: "<<*p<<"State: MOVING DOWN"<<endl;
            (*p)--;
        }
        *(curpos) = out;
        *p = out;

    }


};


int main(int argc,char *argv[])
{
    /*********************Socket Creation ****************/
    struct sockaddr_in serverAddr;
    int socketfd;                      //file descriptor for socket id
    if(argc<2)                         //if portno. is not provided as an argument then out(-1)
    {
        fprintf(stderr,"use %s <port>\n",argv[0]);
        exit(-1);
    }

    int portno =atoi(argv[1]);                                                    //using atoi to read argument as an integer
    socketfd = socket(AF_INET,SOCK_STREAM,0);                                    //using IPv4 connection and TCP protocol
    if(!socketfd)                                                               //socketfd returns 0 if any error in creating socket
    {
        perror("Error in opening socket");
        exit(-1);
    }
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_addr.s_addr=htonl(INADDR_ANY);                                //use this to specify IP of the server or use a local address for tranferring of data
    serverAddr.sin_port=htons(portno);


    if(bind(socketfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr))<0)       //binding of socketfd to the port
    {
        perror("failed to bind");
        exit(4);
    }
    if(listen(socketfd,50))//50 specifies the maximum no. of backlog connections
    {
        perror("error in listen ");
        exit(2);
    }
    int optval=1;
    setsockopt(socketfd,SOL_SOCKET,SO_REUSEADDR,(const void *)&optval,sizeof(int));  // unknown function
    struct sockaddr_in clientAddr;
    socklen_t cliLen=sizeof(clientAddr);


    System sys;
    cout << "\t\t\t ::::: Welcome to the Hotel Transylvania :::::" << endl;
    cout << "========================================================================================" << endl;
    cout << "=====================================System Setup=======================================" << endl;

    cout << "Uppermost floor :::: ";
    cin >> sys.max_floor;
    cout << "Bottommost floor :::: ";
    cin >> sys.min_floor;

    cout << "======================================Lift Setup========================================" << endl;

    cout << "Current position of first lift :::: ";
    cin >> sys.lift1.initial_pos;
    cout << "Current position of second lift :::: ";
    cin >> sys.lift2.initial_pos;




    int newSocket=accept(socketfd,(struct sockaddr *)&clientAddr,&cliLen);
    if(newSocket<0)
    {
        perror("Error in accept");
        exit(-3);
    }

    cout << "========================================================================================" << endl;
    cout << "Please enter the Lift Transport Sequence (Enter 0 0 0 to out)" << endl;
    cout << "______________________________________________________________" << endl;

    input(&newSocket);  //Taking The first input
    sys.lift1.state = 'R'; sys.lift2.state= 'R';
    int i1=0, i2=0;
    int num;
    while (!(enter==0&&trans='0'&&out=0))
    {
        num=nearest(enter,sys.lift1.current_pos,sys.lift2.current_pos);
        switch(num)
        {
            case 1:  //FSM-1
                switch (sys.lift1.state)
                {
                sys.lift1.in[i1]=enter;
                sys.lift1.tr[i1]=trans;
                sys.lift1.out[i1]=out;
                case 'U':
                    sys.transitionup(&sys.lift1.current_pos, sys.lift1.in[i1], sys.lift1.tr[i1], sys.lift1.out[i1], num);
                    sys.lift1.state = 'R';
                    i1++;
                    break;

                case 'D':
                    sys.transitiondown(&sys.lift1.current_pos, sys.lift1.in[i1], sys.lift1.tr[i1], sys.lift1.out[i1], num);
                    sys.lift1.state = 'R';
                    i1++;
                    break;


                case 'R':
                    cout<<"::: Lift 1 is free... Please Enter :::  "<< endl;

                    switch (sys.lift1.tr[i1])
                    {
                    case 'U': sys.lift1.state = 'U';
                        break;
                    case 'D': sys.lift1.state = 'D';
                        break;
                    case 'R': sys.lift1.state = 'R';
                        i1++;
                        break;
                    }
                    break;

                default:
                    cout << "IT IS A LIFT! NOT DORAEMON KA DARWAZA.. Valid input please." << endl;
                }


            case 2:   //FSM-2
                cout<<"Assigned to Lift1: "<<endl;
                switch (sys.liftdown.state)
                {
                sys.lift2.in[i2]=enter;
                sys.lift2.tr[i2]=trans;
                sys.lift2.out[i2]=out;
                case 'U':
                    sys.transitionup(&sys.liftdown.current_pos, sys.liftdown.in[i2], sys.liftdown.tr[i2], sys.liftdown.out[i2],num);
                    sys.liftdown.state = 'R';
                    i2++;
                    break;

                case 'D':
                    sys.transitiondown(&sys.liftdown.current_pos, sys.liftdown.in[i2], sys.liftdown.tr[i2], sys.liftdown.out[i2], num);
                    sys.liftdown.state = 'R';
                    i2++;
                    break;


                case 'R':
                    cout<<"::: Lift 2 is free... Please Enter :::  "<< endl;
                    switch (sys.liftdown.tr[i2])
                    {
                    case 'U': sys.liftdown.state = 'U';
                        break;
                    case 'D': sys.liftdown.state = 'D';
                        break;
                    case 'R': sys.liftdown.state = 'R';
                        i2++;
                        break;
                    }
                    break;

                default:
                    cout << "IT IS A LIFT! NOT DORAEMON KA DARWAZA.. Valid input please." << endl;
                }
        }
        input(&newSocket);
    }

    return 0;
}
