/*
 * File:   main.cpp
 * Author: Diego Perez
 * Purpose: Project 2 - Battleship
 *          Play a simplified game of Battleship while demonstrating
 *          functions, arrays, vectors, searching, sorting, formatted
 *          output, input validation, and file input/output.
 */

//System Libraries
#include <iostream>    //Input/Output Library
#include <iomanip>     //Format Library
#include <cstdlib>     //Random Number Library
#include <ctime>       //Time Library
#include <fstream>     //Read and Write to a File
#include <string>      //String Objects
#include <vector>      //STL Vector Library
#include <cmath>       //Math Library
using namespace std;   //Library Name-space

//User Libraries

//Global/Universal Constants -- No Global Variables
//Science, Math, Conversions, Higher Dimensioned constants only

//Function Prototypes
void intro();
char menu();
void rules();
void init(char [][10],int);
void copy2d(const char [][10],char [][10],int);
bool valid(const char [][10],int,int,int);
bool putshp(char [][10],int,int,int,int);
void place(char [][10],int);
void show(const char [][10],int,bool hide=true);
void show(const string [],const int [],const int [],int);
int getint(string,int,int);
bool attack(char [][10],int,int,vector<int> &);
void aicord(const char [][10],int &,int &,int);
int turns(char mode='R');
int count(const char [][10],int,char);
void play(string &,int &,int &,vector<int> &);
void save(const string [],const int [],const int [],int);
int load(string [],int [],int [],int);
void addrec(string [],int [],int [],int &,string,int,int);
void bubsort(string [],int [],int [],int);
void selsort(string [],int [],int [],int);
int linsrch(const string [],int,string);
int binsrch(const string [],int,string);
void srchlb(string [],int [],int [],int);
void vecout(const vector<int> &);
int calc(int,int);
int calc(int,int,int);
string reslet(int,int);

//Execution Begins Here
int main(int argc,char** argv){
    //Use standard main parameters
    static_cast<void>(argc);
    static_cast<void>(argv);

    //Set the random number seed
    srand(static_cast<unsigned int>(time(0)));

    //Declare variables
    const int MAX=20;            //Maximum leaderboard records
    string names[MAX];           //Parallel array of player names
    int wins[MAX]={};            //Parallel array of player wins
    int games[MAX]={};           //Parallel array of games played
    int nplyr=0;                 //Number of loaded player records
    string pname;                //Name returned from one game
    int gwin=0;                  //Win value returned from one game
    int ggame=0;                 //Game value returned from one game
    vector<int> shots;           //All player shots in this session
    char choice='0';             //Main-menu choice
    bool done=false;             //Controls the main do-while loop

    //Read previous player records
    nplyr=load(names,wins,games,MAX);

    //Display the introduction
    intro();

    //Process the main menu
    do{
        choice=menu();

        //Process the menu choice
        if(choice=='1'){
            //Initialize one-game results
            pname="";
            gwin=0;
            ggame=0;

            //Play game and update player record
            play(pname,gwin,ggame,shots);
            addrec(names,wins,games,nplyr,pname,gwin,ggame);
            save(names,wins,games,nplyr);
        }else if(choice=='2'){
            //Sort and display the leaderboard
            bubsort(names,wins,games,nplyr);
            show(names,wins,games,nplyr);
        }else if(choice=='3'){
            //Search the leaderboard
            srchlb(names,wins,games,nplyr);
        }else if(choice=='4'){
            //Display the rules
            rules();
        }else if(choice=='5'){
            //Display the shot history
            cout<<endl<<"Shot history from the current session"<<endl;
            vecout(shots);
        }else{
            //Save records before quitting
            save(names,wins,games,nplyr);
            cout<<"Records saved. Thank you for playing!"<<endl;
            done=true;
        }
    }while(!done);

    //Exit stage right
    exit(0);
}

void intro(){
    cout<<"============================================"<<endl;
    cout<<"             BATTLESHIP PROJECT 2           "<<endl;
    cout<<"============================================"<<endl;
    cout<<"Sink all five computer ships before the"<<endl;
    cout<<"computer sinks all five of your ships."<<endl;
}

char menu(){
    char pick;

    //Display the menu
    cout<<endl;
    cout<<"1. Play Battleship"<<endl;
    cout<<"2. Display leaderboard"<<endl;
    cout<<"3. Search leaderboard"<<endl;
    cout<<"4. Display rules"<<endl;
    cout<<"5. Display shot history"<<endl;
    cout<<"6. Save and quit"<<endl;
    cout<<"Enter your choice: ";
    cin>>pick;

    //Validate the menu choice
    while(pick<'1'||pick>'6'){
        cout<<"Invalid choice. Enter 1 through 6: ";
        cin>>pick;
    }

    //Return the menu choice
    switch(pick){
        case '1': return '1';
        case '2': return '2';
        case '3': return '3';
        case '4': return '4';
        case '5': return '5';
        default:  return '6';
    }
}

void rules(){
    cout<<endl<<"BATTLESHIP RULES"<<endl;
    cout<<"The board contains ten rows and ten columns."<<endl;
    cout<<"Rows and columns are numbered from 0 through 9."<<endl;
    cout<<"Five ships are placed automatically for each player."<<endl;
    cout<<"The ships occupy 5, 4, 3, 3, and 2 spaces."<<endl;
    cout<<"Enter a row and column to fire at the computer board."<<endl;
    cout<<"An X represents a hit and an O represents a miss."<<endl;
    cout<<"The first side to hit all seventeen ship spaces wins."<<endl;
}

void init(char board[][10],int size){
    //Initialize the game board
    for(int row=0;row<size;row++){
        for(int col=0;col<size;col++){
            board[row][col]='~';
        }
    }
}

void copy2d(const char src[][10],char dest[][10],int size){
    //Copy the two-dimensional array
    for(int row=0;row<size;row++){
        for(int col=0;col<size;col++){
            dest[row][col]=src[row][col];
        }
    }
}

bool valid(const char board[][10],int row,int col,int len){
    bool ok=true;

    //Check the starting coordinate
    if(row<0||row>=10||col<0||col>=10){
        ok=false;
    }

    //Check the right board edge
    if(ok&&col+len>10){
        ok=false;
    }

    //Check for overlapping ships
    for(int pos=0;pos<len&&ok;pos++){
        if(board[row][col+pos]!='~'){
            ok=false;
        }
    }

    return ok;
}

bool putshp(char board[][10],int row,int col,int len,int ship){
    bool ok=valid(board,row,col,len);

    //Place one ship on the board
    if(ok){
        for(int pos=0;pos<len;pos++){
            board[row][col+pos]=static_cast<char>('A'+ship);
        }
    }

    return ok;
}

void place(char board[][10],int size){
    int lens[5]={5,4,3,3,2};
    int nshp=static_cast<int>(sizeof(lens)/sizeof(lens[0]));
    int ship=0;

    //Place all five ships
    while(ship<nshp){
        int row=rand()%size;
        int col=rand()%size;

        if(putshp(board,row,col,lens[ship],ship)){
            ship++;
        }
    }
}

void show(const char board[][10],int size,bool hide){
    //Display column headings
    cout<<"   ";
    for(int col=0;col<size;col++){
        cout<<setw(2)<<col;
    }
    cout<<endl;

    //Display the board
    for(int row=0;row<size;row++){
        cout<<setw(2)<<row<<" ";

        for(int col=0;col<size;col++){
            char cell=board[row][col];

            //Hide unhit computer ships
            cell=(hide&&cell>='A'&&cell<='E')?'~':cell;
            cout<<setw(2)<<cell;
        }
        cout<<endl;
    }
}

void show(const string names[],const int wins[],
          const int games[],int size){
    //Display column headings
    cout<<endl;
    cout<<left<<setw(18)<<"Player"
        <<right<<setw(10)<<"Wins"
        <<setw(10)<<"Games"
        <<setw(10)<<"Rate"<<endl;
    cout<<string(48,'-')<<endl;

    //Display the parallel arrays
    for(int i=0;i<size;i++){
        float rate=0.0f;

        if(games[i]>0){
            rate=static_cast<float>(wins[i])/games[i]*100.0f;
        }

        cout<<left<<setw(18)<<names[i]
            <<right<<setw(10)<<wins[i]
            <<setw(10)<<games[i]
            <<setw(9)<<fixed<<setprecision(1)<<rate<<"%"<<endl;
    }

    //Display message when no records exist
    if(size==0){
        cout<<"No leaderboard records are available."<<endl;
    }
}

int getint(string msg,int low,int high){
    int value;

    cout<<msg;
    cin>>value;

    //Validate numeric input
    while(cin.fail()||value<low||value>high){
        cin.clear();
        cin.ignore(1000,'\n');
        cout<<"Invalid entry. Enter "<<low<<" through "<<high<<": ";
        cin>>value;
    }

    return value;
}

bool attack(char board[][10],int row,int col,vector<int> &shots){
    bool hit=false;
    int code=row*10+col;

    //Store the shot in the vector
    shots.push_back(code);

    //Mark a hit or miss
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

    //Generate an unused computer coordinate
    while(used){
        row=rand()%size;
        col=rand()%size;
        used=board[row][col]=='X'||board[row][col]=='O';
    }
}

int turns(char mode){
    static int total=0;

    if(mode=='A'){
        total++;
    }else if(mode=='Z'){
        total=0;
    }

    return total;
}

int count(const char board[][10],int size,char find){
    int total=0;

    //Search every board location with a simple linear traversal.
    for(int row=0;row<size;row++){
        for(int col=0;col<size;col++){
            if(board[row][col]==find){
                total++;
            }
        }
    }

    return total;
}

int calc(int wins,int games){
    return games-wins;
}

int calc(int pleft,int cleft,int turn){
    return abs(pleft-cleft)+turn;
}

string reslet(int pleft,int cleft){
    string result;

    if(cleft==0){
        result="Player";
    }else if(pleft==0){
        result="Computer";
    }else{
        result="None";
    }

    return result;
}

void play(string &pname,int &gwin,int &ggame,vector<int> &shots){
    //Declare local board size, arrays, and game variables.
    const int SIZE=10;
    char pbrd[SIZE][10];
    char cbrd[SIZE][10];
    char temp[SIZE][10];
    int prow,pcol,crow,ccol;
    int phit=0,chit=0;
    bool over=false;
    vector<int> cshot; //Temporary computer-shot vector for attack()

    //Demonstrate multiple assignment and reset the static counter.
    prow=pcol=crow=ccol=0;
    turns('Z');

    //Read a nonempty player name.
    cin.ignore(1000,'\n');
    cout<<"Enter player name: ";
    getline(cin,pname);

    while(pname==""){
        cout<<"Name cannot be empty. Enter player name: ";
        getline(cin,pname);
    }

    //Initialize both boards and randomly place the ships.
    init(pbrd,SIZE);
    init(cbrd,SIZE);
    place(pbrd,SIZE);
    place(cbrd,SIZE);

    //Demonstrate copying a two-dimensional array through functions.
    copy2d(pbrd,temp,SIZE);
    copy2d(temp,pbrd,SIZE);

    cout<<endl<<"Your ships are shown with letters A through E."<<endl;

    //Repeat complete player/computer turns until all 17 spaces are hit.
    do{
        //Display the player's visible ships and the hidden computer board.
        cout<<endl<<"YOUR BOARD"<<endl;
        show(pbrd,SIZE,false);

        cout<<endl<<"COMPUTER BOARD"<<endl;
        show(cbrd,SIZE);

        //Read and validate the player's attack coordinate.
        prow=getint("Enter attack row 0-9: ",0,9);
        pcol=getint("Enter attack column 0-9: ",0,9);

        //Prevent the player from attacking the same coordinate twice.
        while(cbrd[prow][pcol]=='X'||cbrd[prow][pcol]=='O'){
            cout<<"You already attacked that position."<<endl;
            prow=getint("Enter a different row 0-9: ",0,9);
            pcol=getint("Enter a different column 0-9: ",0,9);
        }

        //Process the player's shot and update the hit counter.
        if(attack(cbrd,prow,pcol,shots)){
            phit++;
            cout<<"Direct hit!"<<endl;
        }else{
            cout<<"Miss."<<endl;
        }

        //Allow the computer to attack only when the player has not won.
        if(phit<17){
            aicord(pbrd,crow,ccol,SIZE);
            cshot.clear();

            if(attack(pbrd,crow,ccol,cshot)){
                chit++;
                cout<<"The computer hit row "<<crow
                    <<", column "<<ccol<<"."<<endl;
            }else{
                cout<<"The computer missed row "<<crow
                    <<", column "<<ccol<<"."<<endl;
            }
        }

        //Increment the static counter and display current game statistics.
        turns('A');

        int pleft=17-chit;
        int cleft=17-phit;
        int alert=calc(pleft,cleft,turns());

        cout<<"Your ship spaces left: "<<pleft<<endl;
        cout<<"Computer ship spaces left: "<<cleft<<endl;
        cout<<"Battle activity value: "<<alert<<endl;

        //The game ends when either board contains seventeen X characters.
        if(count(cbrd,SIZE,'X')>=17||count(pbrd,SIZE,'X')>=17){
            over=true;
        }
    }while(!over);

    //Return exactly one game and either zero or one win to main().
    ggame=1;
    gwin=0;

    if(phit>=17){
        gwin=1;
        cout<<endl<<"You sank every computer ship. You win!"<<endl;
    }else{
        cout<<endl<<"The computer sank every ship. You lose."<<endl;
    }

    //Display the final result of this individual game.
    int lose=calc(gwin,ggame);
    cout<<"This-game wins: "<<gwin<<endl;
    cout<<"This-game losses: "<<lose<<endl;
    cout<<"Total turns taken: "<<turns()<<endl;
    cout<<"Winner check: "<<reslet(17-chit,17-phit)<<endl;
}

int load(string names[],int wins[],int games[],int max){
    ifstream in;
    int size=0;

    //Attempt to open the leaderboard input file.
    in.open("scores.dat");

    //Only read records when the file opened successfully.
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

    //Open the file in output mode, replacing the prior saved data.
    out.open("scores.dat");

    //Write each record as three consecutive lines.
    for(int i=0;i<size;i++){
        out<<names[i]<<endl;
        out<<wins[i]<<endl;
        out<<games[i]<<endl;
    }

    out.close();
}

void addrec(string names[],int wins[],int games[],int &size,
            string name,int win,int game){
    //Use a linear search before adding or updating the record.
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

void bubsort(string names[],int wins[],int games[],int size){
    bool swap=true;
    int pass=0;

    //Continue passes until no swap occurs or all passes are complete.
    while(swap&&pass<size-1){
        swap=false;

        for(int i=0;i<size-1-pass;i++){
            if(wins[i]<wins[i+1]){
                //Swap the wins.
                int itemp=wins[i];
                wins[i]=wins[i+1];
                wins[i+1]=itemp;

                //Swap the matching games to preserve parallel records.
                itemp=games[i];
                games[i]=games[i+1];
                games[i+1]=itemp;

                //Swap the matching names to preserve parallel records.
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
    //Move the smallest remaining name into each starting position.
    for(int start=0;start<size-1;start++){
        int small=start;

        for(int i=start+1;i<size;i++){
            if(names[i]<names[small]){
                small=i;
            }
        }

        //Only perform the three parallel swaps when needed.
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

    //Stop when the item is found or every record has been checked.
    while(i<size&&pos==-1){
        if(names[i]==find){
            pos=i;
        }
        i++;
    }

    return pos;
}

int binsrch(const string names[],int size,string find){
    int first=0;
    int last=size-1;
    int pos=-1;

    //Continue while a searchable range remains and no match was found.
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

    //Avoid reading a search value when no records exist.
    if(size==0){
        cout<<"No records are available to search."<<endl;
        return;
    }

    //Read the exact name, including spaces.
    cin.ignore(1000,'\n');
    cout<<"Enter the exact player name to search for: ";
    getline(cin,find);

    //Search once before sorting, then sort and search again.
    lpos=linsrch(names,size,find);
    selsort(names,wins,games,size);
    bpos=binsrch(names,size,find);

    cout<<"Linear search position before sorting: "<<lpos<<endl;
    cout<<"Binary search position after sorting: "<<bpos<<endl;

    //Display either the matching record or a not-found message.
    if(bpos>=0){
        cout<<"Player: "<<names[bpos]<<endl;
        cout<<"Wins: "<<wins[bpos]<<endl;
        cout<<"Games: "<<games[bpos]<<endl;
    }else{
        cout<<"Player was not found."<<endl;
    }
}

void vecout(const vector<int> &shots){
    //Display a message when the player has not fired any shots.
    if(shots.size()==0){
        cout<<"No player shots have been recorded."<<endl;
    }else{
        //Decode each integer into its original row and column.
        for(unsigned int i=0;i<shots.size();i++){
            int row=shots[i]/10;
            int col=shots[i]%10;

            cout<<"Shot "<<setw(3)<<i+1
                <<": row "<<row<<", column "<<col<<endl;
        }
    }
}
