/*
 * File:   main.cpp
 * Author: Diego Perez
 * Purpose: Project 2 Battleship - Version 2
 *          Adds ships, attacks, validation, and basic gameplay.
 */

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;   //Library Name-space

//Function Prototypes
void intro();
char menu();
void rules();
void init(char [][10],int);
void show(const char [][10],int,bool hide=true);
bool valid(const char [][10],int,int,int);
bool putshp(char [][10],int,int,int,int);
void place(char [][10],int);
int getint(string,int,int);
bool attack(char [][10],int,int);
void aicord(const char [][10],int &,int &,int);
void play();

int main(int argc,char** argv){
    srand(static_cast<unsigned int>(time(0)));

    char choice='0';
    bool done=false;

    intro();

    do{
        choice=menu();

        if(choice=='1'){
            play();
        }else if(choice=='2'){
            rules();
        }else if(choice=='3'){
            cout<<"Version 2 complete. Goodbye!"<<endl;
            done=true;
        }
    }while(!done);

    return 0;
}

void intro(){
    cout<<"============================================"<<endl;
    cout<<"             BATTLESHIP PROJECT 2           "<<endl;
    cout<<"                 VERSION 2                  "<<endl;
    cout<<"============================================"<<endl;
}

char menu(){
    char pick;

    cout<<endl;
    cout<<"1. Play a basic game"<<endl;
    cout<<"2. Display rules"<<endl;
    cout<<"3. Quit"<<endl;
    cout<<"Enter your choice: ";
    cin>>pick;

    while(pick<'1'||pick>'3'){
        cout<<"Invalid choice. Enter 1 through 3: ";
        cin>>pick;
    }

    return pick;
}

void rules(){
    cout<<endl<<"BATTLESHIP RULES"<<endl;
    cout<<"Rows and columns are numbered 0 through 9."<<endl;
    cout<<"Five ships are placed on each board."<<endl;
    cout<<"An X is a hit and an O is a miss."<<endl;
    cout<<"Version 2 ends after ten player turns."<<endl;
}

void init(char board[][10],int size){
    for(int row=0;row<size;row++){
        for(int col=0;col<size;col++){
            board[row][col]='~';
        }
    }
}

void show(const char board[][10],int size,bool hide){
    cout<<"   ";
    for(int col=0;col<size;col++){
        cout<<setw(2)<<col;
    }
    cout<<endl;

    for(int row=0;row<size;row++){
        cout<<setw(2)<<row<<" ";

        for(int col=0;col<size;col++){
            char cell=board[row][col];

            if(hide&&cell>='A'&&cell<='E'){
                cell='~';
            }

            cout<<setw(2)<<cell;
        }
        cout<<endl;
    }
}

bool valid(const char board[][10],int row,int col,int len){
    bool ok=true;

    if(row<0||row>=10||col<0||col>=10){
        ok=false;
    }

    if(ok&&col+len>10){
        ok=false;
    }

    for(int pos=0;pos<len&&ok;pos++){
        if(board[row][col+pos]!='~'){
            ok=false;
        }
    }

    return ok;
}

bool putshp(char board[][10],int row,int col,int len,int ship){
    bool ok=valid(board,row,col,len);

    if(ok){
        for(int pos=0;pos<len;pos++){
            board[row][col+pos]=static_cast<char>('A'+ship);
        }
    }

    return ok;
}

void place(char board[][10],int size){
    int lens[5]={5,4,3,3,2};
    int ship=0;

    while(ship<5){
        int row=rand()%size;
        int col=rand()%size;

        if(putshp(board,row,col,lens[ship],ship)){
            ship++;
        }
    }
}

int getint(string msg,int low,int high){
    int value;

    cout<<msg;
    cin>>value;

    while(cin.fail()||value<low||value>high){
        cin.clear();
        cin.ignore(1000,'\n');
        cout<<"Invalid entry. Enter "<<low<<" through "<<high<<": ";
        cin>>value;
    }

    return value;
}

bool attack(char board[][10],int row,int col){
    bool hit=false;

    if(board[row][col]>='A'&&board[row][col]<='E'){
        board[row][col]='X';
        hit=true;
    }else if(board[row][col]=='~'){
        board[row][col]='O';
    }

    return hit;
}

void aicord(const char board[][10],int &row,int &col,int size){
    bool used=true;

    while(used){
        row=rand()%size;
        col=rand()%size;
        used=board[row][col]=='X'||board[row][col]=='O';
    }
}

void play(){
    const int SIZE=10;
    char pbrd[SIZE][10];
    char cbrd[SIZE][10];
    int prow,pcol,crow,ccol;
    int turn=0;

    init(pbrd,SIZE);
    init(cbrd,SIZE);
    place(pbrd,SIZE);
    place(cbrd,SIZE);

    while(turn<10){
        cout<<endl<<"YOUR BOARD"<<endl;
        show(pbrd,SIZE,false);

        cout<<endl<<"COMPUTER BOARD"<<endl;
        show(cbrd,SIZE);

        prow=getint("Enter attack row 0-9: ",0,9);
        pcol=getint("Enter attack column 0-9: ",0,9);

        while(cbrd[prow][pcol]=='X'||cbrd[prow][pcol]=='O'){
            cout<<"That position was already attacked."<<endl;
            prow=getint("Enter another row: ",0,9);
            pcol=getint("Enter another column: ",0,9);
        }

        cout<<(attack(cbrd,prow,pcol)?"Direct hit!":"Miss.")<<endl;

        aicord(pbrd,crow,ccol,SIZE);
        cout<<(attack(pbrd,crow,ccol)?"Computer hit!":"Computer missed.")
            <<endl;

        turn++;
    }

    cout<<"The ten-turn Version 2 demonstration is complete."<<endl;
}
