/*
 * File:   main.cpp
 * Author: Diego Perez
 * Purpose: Project 2 Battleship - Version 1
 *          Program structure, menu, and board display.
 */

#include <iostream>
#include <iomanip>
#include <string>
using namespace std;   //Library Name-space

//Function Prototypes
void intro();
char menu();
void rules();
void init(char [][10],int);
void show(const char [][10],int,bool hide=true);

int main(int argc,char** argv){
    const int SIZE=10;
    char board[SIZE][10];
    char choice='0';
    bool done=false;

    init(board,SIZE);
    intro();

    do{
        choice=menu();

        if(choice=='1'){
            cout<<endl<<"Empty Battleship Board"<<endl;
            show(board,SIZE,false);
        }else if(choice=='2'){
            rules();
        }else if(choice=='3'){
            cout<<"Version 1 complete. Goodbye!"<<endl;
            done=true;
        }
    }while(!done);

    return 0;
}

void intro(){
    cout<<"============================================"<<endl;
    cout<<"             BATTLESHIP PROJECT 2           "<<endl;
    cout<<"                 VERSION 1                  "<<endl;
    cout<<"============================================"<<endl;
}

char menu(){
    char pick;

    cout<<endl;
    cout<<"1. Display board"<<endl;
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
    cout<<"The board has ten rows and ten columns."<<endl;
    cout<<"The player selects a row and column to attack."<<endl;
    cout<<"The goal is to sink every opposing ship."<<endl;
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
