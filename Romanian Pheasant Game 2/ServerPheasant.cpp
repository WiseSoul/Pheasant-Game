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
#include <signal.h>
#include <sys/wait.h>

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

void CopyRightWord(char initialWord[1000],char Word[1000])
{
    strcpy(Word,"");
    char c[3]="";

    for(int i=0;i<strlen(initialWord)-1;i++)
    {
     c[0]=initialWord[i];
     strcat(Word,c);
    }
}

bool ValidateDictionary()
{
	bool valid = 1;
	ifstream fin;

	fin.open("dictionary.txt");

	if(fin.fail())
        valid = 0;

return valid;
}

char PickRandomLetter()
{
    srand (time(NULL));
    int n = rand() % 26;
    char c = (char)(n+65);
    c = c + 32;
}

bool VerifyWinner(char Word[])
{
  char lastLetters[100][3] = {"nt","ng","ps","ft","rd","ns","rt","rn","ls","nd","ee","rh","lt","mn","nc","mp"};
  int len = 16;
  char C[4] = "";

  C[0] = Word[strlen (Word)-2];
  C[1] = Word[strlen (Word)-1];

  for(int i=0;i<len;i++)
  {
      if (strcmp (C,lastLetters[i]) == 0)
        return true;
  }

return false;
}

int getNrOfPlayers()
{
    int number;    
    ifstream fin("nrOfPlayers.txt");

    fin>>number;
    fin.close();
    
	return number;
}


void sighandler(int signo)
{
  if (signo == SIGCHLD) {
	cout<<"[server-"<<getpid()<<"]All players have been disconnected. Free session available.\n";
    while( waitpid(-1, NULL, WNOHANG) > 0  ) ;
  }
}

#define PORT 9024

int main ()
{
    if (ValidateDictionary() == 0) {
		return 0;
	}

    int nrOfPlayers = getNrOfPlayers();
    struct sockaddr_in gameServer;
    struct sockaddr_in from;
    char pWord[100],pAux[100];
    int socketServer;
    int optVal = 1;
    int pid;

	char rand,command[100] = "kill";
	rand = PickRandomLetter();

	while(rand == 'w' || rand == 'y' || rand =='x' || rand == 'q' || rand == 'k')
	rand = PickRandomLetter();

    if ((socketServer = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        cout<< "[server]Error creating socket.\n";
        return 0;
    }

    setsockopt(socketServer, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal));

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

    signal(SIGCHLD, sighandler);


    cout<<endl<<endl;
    cout<<"'||'''|, '||                                             ||       .|'''''|"<<endl;
    cout<<" ||   ||  ||                                             ||       || .                              "<<endl;
    cout<<" ||...|'  ||''|, .|''|,  '''|.  (''''  '''|.  `||''|,  ''||''     || |''||  '''|.  '||),,(|,  .|''|,"<<endl;
    cout<<" ||       ||  || ||..|| .|''||   `'') .|''||   ||  ||    ||       ||    || .|''||   || || ||  ||..||"<<endl;
    cout<<".||      .||  || `|...  `|..||. `...' `|..||. .||  ||.   `|..'    `|....|' `|..||. .||    ||. `|... "<<endl<<endl;


    cout << "                                                                  made by Silviu Patras"<<endl<<endl<<endl<<endl;


    cout<<"Waiting for a number of "<<nrOfPlayers<<" players to connect at port: "<<PORT<<" ..."<<endl;
    int i,j = nrOfPlayers,winner;
    char p1InitWord[100] = "Because you are Player #1 you must write a word that starts with the letter: ";

    p1InitWord[strlen(p1InitWord)] = rand;

    while (1)
    {
        int player[nrOfPlayers+1];
        int size = sizeof (from);
		char address[INET_ADDRSTRLEN];
		char checkLast[100] = "";
		fflush (stdout);

	for(i=0;i<nrOfPlayers;i++) 
	{

	player[i] = accept (socketServer, (struct sockaddr *) &from,(socklen_t*) &size);

        if (player[i] < 0)
        {
            cout<< "[server]Error at accept().\n";
            continue;
        }
	else
        {
          inet_ntop(AF_INET, &from.sin_addr.s_addr, address, INET_ADDRSTRLEN);
    	  cout<< "Player:"<<i+1<<" connected to [server-"<<getpid()<<"] at adress:"<<address<<" "<<ntohs(from.sin_port)<<endl;
        }

	}

	if ( (pid = fork()) < 0 ) {
	cout<<"Fork error";
	continue;
        }
        else if (pid == 0) {

       cout<<"Starting game ..."<<endl<<endl;

	   cout<<"[server-"<<getpid()<<"] logs:"<<endl;

	   write(player[0], p1InitWord, 100);

	   if (read (player[0], pWord, 100) <= 0)
        {
            cout<< "Player #"<<'1'<<" has entered a invalid word or has disconnected."<<endl;

        }

		CopyRightWord(pWord,checkLast);

	if (VerifyWinner(checkLast) == 1)
		cout<<"Player #1 has closed the game from the first round. He lost!"<<endl;

		cout<<"Player #1's '"<<checkLast<<"'";

	  if (ValidateWord(checkLast) == 1 && checkLast[0] == rand){
		cout<<" is a valid word that starts with letter: "<<rand<<endl;
		}

	  else{
	    cout<<" is not a valid word. He has been disconnected."<<endl;
		}

	   while(1) {

	    if (j == 0){

		 if (read (player[j], pWord, 100) <= 0){
            cout<< "Player #"<<j+1<<" has entered a invalid word or has disconnected."<<endl;
           }

		CopyRightWord(pWord,pAux);

		cout<<"Player #"<<j+1<<"'s '"<<pAux<<"' ";

		if (ValidateWord(pAux) == 1 && pAux[0] == checkLast[strlen(checkLast)-2] && pAux[1] == checkLast[strlen(checkLast)-1]) {
		cout<<"is a valid word that starts with letters: "<<pAux[0]<<pAux[1]<<endl;
		   }
		else {
		cout<<"is not a valid word. He has been disconnected."<<endl;

		}

		if (VerifyWinner(pAux) == 1)
			winner = j;

		strcpy(checkLast,pAux);
		}

	   for(j=1;j<nrOfPlayers;j++)
	   {
		if (write(player[j], pWord, 100) <=0){
		   cout<<"Cannot send word to Player #"<<j<<endl;
		   return 0;
		}

		 if (read (player[j], pWord, 100) <= 0){
                 cout<< "Player #"<<j<<" has entered a invalid word or has disconnected."<<endl;
                 }

		CopyRightWord(pWord,pAux);

		cout<<"Player #"<<j+1<<"'s '"<<pAux<<"' ";

		if (ValidateWord(pAux) == 1 && pAux[0] == checkLast[strlen(checkLast)-2] && pAux[1] == checkLast[strlen(checkLast)-1]) {
		cout<<"is a valid word that starts with letters: "<<pAux[0]<<pAux[1]<<endl;
		}

	    else{
	    cout<<"is not a valid word. He has been disconnected."<<endl;
		}

		if (VerifyWinner(pAux) == 1)
			winner = j;

		strcpy(checkLast,pAux);	
	   }

	  if(j == nrOfPlayers){

		if (write(player[0], pWord, 100) <=0){
		   cout<<"Cannot send word to Player #"<<j-1<<endl;
		   return 0;
		}
		j = 0;
	  }

	}

   }
  }
}
