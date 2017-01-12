#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;

char PickRandomLetter()
{
    srand (time(NULL));
    int n = rand() % 26;
    char c = (char)(n+65);
    c = c + 32;
}

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


bool ShowWinner(char win[])
{
   ifstream fin("winner.txt");
   
   strcpy(win,"");

   fin.get(win,50);
   fin.close();

   if(strlen(win) > 10)
	return true;

return false;
}

void Clear()
{
    
    ofstream clr;
    
    clr.open("winner.txt", ofstream::out | ofstream::trunc);
    
    clr.close();

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

int port;
int ply = 0;

int main (int argc, char *argv[])
{

    int socketDescr;
    struct sockaddr_in server;
    char word[100]="",auxWord[100] = "";
    char fChr,lChr;
    char winner[100] = "";	

    if (argc != 3)
    {
        printf ("Sintax is: %s <server_adress> <port>\n", argv[0]);
        return -1;
    }

    port = atoi (argv[2]);

    if ((socketDescr = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        cout<<"Error creating socket."<<endl;
        return 0;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_port = htons (port);

    if (connect (socketDescr, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
	cout<<"Error connecting to the server.";
        return 0;
    }
	
    cout<<endl<<endl;
    cout<<"'||'''|, '||                                             ||       .|'''''|"<<endl;
    cout<<" ||   ||  ||                                             ||       || .                              "<<endl;
    cout<<" ||...|'  ||''|, .|''|,  '''|.  (''''  '''|.  `||''|,  ''||''     || |''||  '''|.  '||),,(|,  .|''|,"<<endl;
    cout<<" ||       ||  || ||..|| .|''||   `'') .|''||   ||  ||    ||       ||    || .|''||   || || ||  ||..||"<<endl;
    cout<<".||      .||  || `|...  `|..||. `...' `|..||. .||  ||.   `|..'    `|....|' `|..||. .||    ||. `|... "<<endl<<endl;

    cout << "                                                                   made by Silviu Patras"<<endl<<endl<<endl<<endl;


    cout<<"Waiting for the other players to connect ..."<<endl<<endl;

    char pWord[100] = "";
	char aux[100] = "";
	char check[100] = "";
	
    while(1)
    {
      if (ShowWinner(winner) == true)
    	{ 
	cout<<winner<<"!"<<endl;
	Clear();
	return 0;
    	}		

	bzero(word,100);
	read (socketDescr, word, 100);
	strcpy(aux,word);
	
	if (strlen(word) < 20)
	cout<<"The player before you :";

	cout<<word<<endl;
	
	if (strlen(word) < 20) {
	cout<<"You have to write a word that start with the letters: "<<word[strlen(word)-3]<<word[strlen(word)-2]<<endl<<endl;
	fChr = word[strlen(word)-3];
	lChr = word[strlen(word)-2];
	}

	cout<<"Enter word:";

	bzero(word, 100);
        fflush(stdout);
        read(0, word, 100);

	CopyRightWord(word,auxWord);

	if(strlen(aux)>20)
	{
		if (ValidateWord(auxWord) == 1) {
			cout<<"Valid word. Next player's turn."<<endl<<endl;
		}
		else {
			cout<<"You entered a invalid word. You have been disconnected."<<endl;
			close(socketDescr);
			return 0;
		}
	}
	
	else {
	if (ValidateWord(auxWord) == 1 && auxWord[0] == fChr && auxWord[1] == lChr) {
	cout<<"Valid word. Next player's turn."<<endl<<endl;
	}
	else if (ShowWinner(winner) == false) {
	cout<<"You entered a invalid word. You have been disconnected."<<endl;
	close(socketDescr);
	return 0;
	}
	}

    if (write (socketDescr, word, 100) <= 0)
    {
       cout<<"Error sending word to server"<<endl;
       return 0;
    }

	
    if (VerifyWinner(auxWord) == 1)
    {		
       cout<<"Congratulations! You have closed the game and won!"<<endl;
       return 0;
    }
	
    CopyRightWord(aux,check);
   
   }
}
