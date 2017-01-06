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

int port;

int main (int argc, char *argv[])
{

    int socketDescr;
    struct sockaddr_in server;
    char word[100];


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
   
    cout<<"Player #1 connected. Starting game ...";
    cout<<endl<<endl<<endl;
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

    cout << "                                                                         made by Silviu Patras"<<endl<<endl<<endl<<endl;

    char randomLetter = PickRandomLetter();
    char p2Word[100] = "",p1Word[100] = "";
   
       cout << "Because you are Player 1,you have to write a word that starts with lettter: "<<randomLetter<<" ..."<<endl<<endl;

       bzero(word, 100);
  
       read(0, word, 100);

       CopyRightWord(word,p1Word);

       if (ValidateWord(p1Word) == 0 || p1Word[0] != randomLetter) {
       cout << "You entered a invalid word.You lost!"<<endl;
       close (socketDescr);
       return 0;
       }
       else { cout<<"Valid word."<<endl<<endl;
	goto loop;
	 }

    while(1)
    {
	cout <<"Player #1:";

	bzero(word, 100);
        fflush(stdout);
        read(0, word, 100);

 	CopyRightWord(word,p1Word);
 
	if (ValidateWord(p1Word) == 1 && p1Word[0] == p2Word[strlen(p2Word)-2] && p1Word[1] == p2Word[strlen(p2Word)-1])
	cout<<"Valid word."<<endl<<endl;
	else {
	cout<<endl<<"Player #1 has entered a invalid word"<<endl<<"Player #2 Wins!"<<endl;
	close (socketDescr);
	return 0;
	     }
	
	loop:
        if (write (socketDescr, word, 100) <= 0)
        {
            cout<<"Error sending word to Player #2."<<endl;
            return 0;
        } 

	cout<<"Waiting for Player #2's word..."<<endl<<endl;

        if (read (socketDescr, word, 100) < 0)
        {
	cout<<"Error reading word from Player #2"<<endl;
	return 0;
        }

	cout << "Player #2:"<<word<<endl;

	CopyRightWord(word,p2Word);

	if (ValidateWord(p2Word) == 1 && p2Word[0] == p1Word[strlen(p1Word)-2] && p2Word[1] == p1Word[strlen(p1Word)-1]) {
	}
	else { 
	cout<<"Player #2 has entered a invalid word."<<endl<<"Player #1 Wins!"<<endl;
	close (socketDescr);
	return 0;
	}
	cout<<"You have to enter a word that start with the letters: "<<word[strlen(word)-3]<<word[strlen(word)-2]<<endl<<endl;

    }
    
    close (socketDescr);
}

