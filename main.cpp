/*
 * File:   main.cpp
 * Author: Diego Perez
 * Purpose: Project 2 Battleship - Version 3
 *          Adds player records, files, searching, and sorting.
 */

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
using namespace std;   //Library Name-space

//Function Prototypes
void intro();
char menu();
int getint(string,int,int);
void play(string &,int &,int &);
int load(string [],int [],int [],int);
void save(const string [],const int [],const int [],int);
void addrec(string [],int [],int [],int &,string,int,int);
void show(const string [],const int [],const int [],int);
void bubsort(string [],int [],int [],int);
void selsort(string [],int [],int [],int);
int linsrch(const string [],int,string);
int binsrch(const string [],int,string);
void srchlb(string [],int [],int [],int);

int main(int argc,char** argv){
    srand(static_cast<unsigned int>(time(0)));

    const int MAX=20;
    string names[MAX];
    int wins[MAX]={};
    int games[MAX]={};
    int nplyr=load(names,wins,games,MAX);
    string pname;
    int pwin=0,pgame=0;
    char choice='0';
    bool done=false;

    intro();

    do{
        choice=menu();

        if(choice=='1'){
            play(pname,pwin,pgame);
            addrec(names,wins,games,nplyr,pname,pwin,pgame);
        }else if(choice=='2'){
            bubsort(names,wins,games,nplyr);
            show(names,wins,games,nplyr);
        }else if(choice=='3'){
            srchlb(names,wins,games,nplyr);
        }else if(choice=='4'){
            save(names,wins,games,nplyr);
            cout<<"Records saved. Goodbye!"<<endl;
            done=true;
        }
    }while(!done);

    return 0;
}

void intro(){
    cout<<"============================================"<<endl;
    cout<<"             BATTLESHIP PROJECT 2           "<<endl;
    cout<<"                 VERSION 3                  "<<endl;
    cout<<"============================================"<<endl;
    cout<<"This version demonstrates player records."<<endl;
}

char menu(){
    char pick;

    cout<<endl;
    cout<<"1. Simulate a game result"<<endl;
    cout<<"2. Display leaderboard"<<endl;
    cout<<"3. Search leaderboard"<<endl;
    cout<<"4. Save and quit"<<endl;
    cout<<"Enter your choice: ";
    cin>>pick;

    while(pick<'1'||pick>'4'){
        cout<<"Invalid choice. Enter 1 through 4: ";
        cin>>pick;
    }

    return pick;
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

void play(string &pname,int &pwin,int &pgame){
    int result;

    cin.ignore(1000,'\n');
    cout<<"Enter player name: ";
    getline(cin,pname);

    while(pname==""){
        cout<<"Name cannot be empty. Enter player name: ";
        getline(cin,pname);
    }

    result=getint("Enter simulated result, 1=win or 0=loss: ",0,1);
    pwin=result;
    pgame=1;

    cout<<(result==1?"Game recorded as a win.":"Game recorded as a loss.")
        <<endl;
}

int load(string names[],int wins[],int games[],int max){
    ifstream in;
    int size=0;

    in.open("scores.dat");

    if(in){
        while(size<max&&getline(in,names[size])){
            in>>wins[size];
            in>>games[size];
            in.ignore(1000,'\n');
            size++;
        }
        in.close();
    }

    return size;
}

void save(const string names[],const int wins[],
          const int games[],int size){
    ofstream out;
    out.open("scores.dat");

    for(int i=0;i<size;i++){
        out<<names[i]<<endl;
        out<<wins[i]<<endl;
        out<<games[i]<<endl;
    }

    out.close();
}

void addrec(string names[],int wins[],int games[],int &size,
            string name,int win,int game){
    int pos=linsrch(names,size,name);

    if(pos>=0){
        wins[pos]+=win;
        games[pos]+=game;
    }else if(size<20){
        names[size]=name;
        wins[size]=win;
        games[size]=game;
        size++;
    }
}

void show(const string names[],const int wins[],
          const int games[],int size){
    cout<<endl;
    cout<<left<<setw(18)<<"Player"
        <<right<<setw(10)<<"Wins"
        <<setw(10)<<"Games"<<endl;
    cout<<string(38,'-')<<endl;

    for(int i=0;i<size;i++){
        cout<<left<<setw(18)<<names[i]
            <<right<<setw(10)<<wins[i]
            <<setw(10)<<games[i]<<endl;
    }

    if(size==0){
        cout<<"No records are available."<<endl;
    }
}

void bubsort(string names[],int wins[],int games[],int size){
    bool swap=true;
    int pass=0;

    while(swap&&pass<size-1){
        swap=false;

        for(int i=0;i<size-1-pass;i++){
            if(wins[i]<wins[i+1]){
                int itemp=wins[i];
                wins[i]=wins[i+1];
                wins[i+1]=itemp;

                itemp=games[i];
                games[i]=games[i+1];
                games[i+1]=itemp;

                string stemp=names[i];
                names[i]=names[i+1];
                names[i+1]=stemp;
                swap=true;
            }
        }
        pass++;
    }
}

void selsort(string names[],int wins[],int games[],int size){
    for(int start=0;start<size-1;start++){
        int small=start;

        for(int i=start+1;i<size;i++){
            if(names[i]<names[small]){
                small=i;
            }
        }

        if(small!=start){
            string stemp=names[start];
            names[start]=names[small];
            names[small]=stemp;

            int itemp=wins[start];
            wins[start]=wins[small];
            wins[small]=itemp;

            itemp=games[start];
            games[start]=games[small];
            games[small]=itemp;
        }
    }
}

int linsrch(const string names[],int size,string find){
    int pos=-1;
    int i=0;

    while(i<size&&pos==-1){
        if(names[i]==find){
            pos=i;
        }
        i++;
    }

    return pos;
}

int binsrch(const string names[],int size,string find){
    int first=0,last=size-1,pos=-1;

    while(first<=last&&pos==-1){
        int mid=(first+last)/2;

        if(names[mid]==find){
            pos=mid;
        }else if(names[mid]<find){
            first=mid+1;
        }else{
            last=mid-1;
        }
    }

    return pos;
}

void srchlb(string names[],int wins[],int games[],int size){
    string find;
    int lpos,bpos;

    if(size==0){
        cout<<"No records are available."<<endl;
        return;
    }

    cin.ignore(1000,'\n');
    cout<<"Enter exact player name: ";
    getline(cin,find);

    lpos=linsrch(names,size,find);
    selsort(names,wins,games,size);
    bpos=binsrch(names,size,find);

    cout<<"Linear search position: "<<lpos<<endl;
    cout<<"Binary search position: "<<bpos<<endl;
}
