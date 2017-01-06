#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <cmath>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

using namespace std;

bool ValidateWord(char word[])
{
    ifstream fin("dictionary.txt");

    char readWord[100] = "";

        while(fin>>readWord)
        {
            if( strcmp(readWord,word) == 0)
	    {
	    fin.close(); 
            return true;
	    }
        }

return false;
}

void CopyRightWord(char initialWord[],char Word[])
{
    strcpy(Word,"");
    char c[3]="";

    for(int i=0;i<strlen(initialWord)-1;i++)
    {
     c[0]=initialWord[i];
     strcat(Word,c);
    }
}

#define PORT 9024

int main ()
{
    struct sockaddr_in gameServer;
    struct sockaddr_in from;
    char p1Word[100],p1Aux[100];
    char p2Word[100],p2Aux[100];
    int socketServer;

    if ((socketServer = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        cout<< "[server]Error creating socket.\n";
        return 0;
    }

    bzero (&gameServer, sizeof (gameServer));
    bzero (&from, sizeof (from));

    gameServer.sin_family = AF_INET;	
    gameServer.sin_addr.s_addr = htonl (INADDR_ANY);
    gameServer.sin_port = htons (PORT);

    
    if (bind (socketServer, (struct sockaddr *) &gameServer, sizeof (struct sockaddr)) == -1)
    {
        cout<< "[server]Error at bind().Use another port.\n";
        return 0;
    }
    
    if (listen (socketServer, 4) == -1)
    {
        cout<< "[server]Error at listen().\n";
        return 0;
    }

    cout<<"Waiting for a player to connect at port: "<<PORT<<" ..."<<endl;
	

    while (1)
    {
        int player;
        int size = sizeof (from);

        fflush (stdout);

        player = accept (socketServer, (struct sockaddr *) &from,(socklen_t*) &size);
        if (player < 0)
        {
            cout<< "[server]Error at accept().\n";
            continue;
        }
	cout<< "Player #2 connected. Starting game..."<<endl<<endl;


	cout<<endl<<endl;
	 cout<<"'||'''|, '||                                             ||       .|'''''|"<<endl;
       	 cout<<" ||   ||  ||                                             ||       || .                              "<<endl;
         cout<<" ||...|'  ||''|, .|''|,  '''|.  (''''  '''|.  `||''|,  ''||''     || |''||  '''|.  '||),,(|,  .|''|,"<<endl;
         cout<<" ||       ||  || ||..|| .|''||   `'') .|''||   ||  ||    ||       ||    || .|''||   || || ||  ||..||"<<endl;
         cout<<".||      .||  || `|...  `|..||. `...' `|..||. .||  ||.   `|..'    `|....|' `|..||. .||    ||. `|... "<<endl<<endl;


        cout<<"                                             ||               ||   "<<endl;
        cout<<"                                            '||              '||  "<<endl;
        cout<<"                                             ||  \\  // (''''  ||  "<<endl;
        cout<<"                                             ||   \\//   `'')  ||  "<<endl;
        cout<<"                                            .||.   \/   `...' .||. "<<endl;



       
        cout<<"                                                                           made by Silviu Patras"<<endl<<endl;
	cout<<"You are Player #2."<<endl;
        cout<<"Waiting for Player #1's word..."<<endl<<endl;
        
        if (read (player, p1Word, 100) <= 0)
            {
                cout<< "Player #1 has entered a invalid word or has disconnected. You win!"<<endl;
                close (player);
                return  0;
            }
	  
	    CopyRightWord(p1Word,p1Aux);

            cout<<"Player #1:"<<p1Word<<endl;

	    if (ValidateWord(p1Aux) == 1){
		goto loop;
		}

	    else{
	        cout<<endl<<"Player #1 has entered a invalid word."<<endl<<"Player #2 Wins!"<<endl;
		close(player);
		close(socketServer);
		return 0;
		}

        while (1)
        {   cout<<"Waiting for Player #1's word."<<endl<<endl;
            
            fflush (stdout);

            if (read (player, p1Word, 100) <= 0)
            {
                cout<< "Player #1 has entered a invalid word or has disconnected. You win!"<<endl;
                close (player);
                return  0;
            }

	    CopyRightWord(p1Word,p1Aux);

            cout<<"Player #1:"<<p1Word<<endl;

	    if (ValidateWord(p1Aux) == 1 && p1Aux[0] == p2Aux[strlen(p2Aux)-2] && p1Aux[1] == p2Aux[strlen(p2Aux)-1]){
		}

	    else{
		close(player);
		close(socketServer);
		return 0;
		}

 	    loop:
	    cout<<"You have to write a word that begins with: "<< p1Word[strlen(p1Word)-3] << p1Word[strlen(p1Word)-2] <<endl<<endl;
            cout<<"Player #2:";

            bzero(p2Word,100);
            fflush(stdout);
            read(0, p2Word, 100);

            if (write (player, p2Word, 100) <= 0)
            {
                cout<< "[server]Error at write() p2Word.\n";
                return 0;
            }

	    CopyRightWord(p2Word,p2Aux);

    	    if (ValidateWord(p2Aux) == 1 && p2Aux[0] == p1Aux[strlen(p1Aux)-2] && p2Aux[1] == p1Aux[strlen(p1Aux)-1]){
		cout<<"Valid word."<<endl<<endl;
		}
	    else{
		cout<<endl<<"Player #2 has entered a invalid word."<<endl<<"Player #1 Wins!"<<endl;
		close(player);
		close(socketServer);
		return 0;
		}

        }
     
    }				
}				

