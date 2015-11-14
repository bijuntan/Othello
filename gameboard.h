#ifndef _gameboard_H
#define _gameboard_H
#include <time.h>
#include <iostream.h>
#include <iomanip.h>
#include <stdlib.h>
#include <stdio.h>
#include <climits>
#include <math.h>
#include <fstream.h>

#define  INF 1000000
#define  MAX(a,b) (((a)>(b))?(a):(b)) 
#define  MIN(a,b) (((a)<(b))?(a):(b)) 

#define BLACK		0
#define WHITE		1

#define UP			0
#define DOWN		1
#define LEFT		2
#define RIGHT		3
#define DIAG_45		4
#define DIAG_135	5
#define DIAG_225	6
#define DIAG_315	7

//Weighting functions: 
#define C	 -50		//weight of c-blocks
#define X	 -500		//weight of x-blocks
#define CN	 1000	//weight of corner-blocks
#define E	 200 	//weight of edge
#define M	 6		//weight of mobility
#define P    6		//weight of computer pieces on the board

class board{
	protected:
		static char pieces[18][19];
		static bool  complete;
		static int	count[2];
		static time_t  timer;
		static bool current_player;							//0->black's turn  1->white's turn
		static bool player_color;							//the color of the user: 0->black, 1->white
		static int  nextmove[2];							//x,y coordinate of the user move
		static int  legal_move_x[60], legal_move_y[60];		//points to array of all possible legal moves
		static int  number_legal_moves;						//the total number of legal moves
		static char p1, p2;									//p1 is computer, p2 is user, 'b'->black, 'w'->white		
 		static int  convert[8];
		static time_t end_time;
		static int  h[60];
		static int  level;
		static time_t current;

		struct state{
			char pieces[18][19];
			int	count[2];
			int  timer;
			bool current_player;	
			int  nextmove[2];
			int  legal_move_x[60], legal_move_y[60];
			int  number_legal_moves;
			int  convert[8];
		};

	public:
		board(void);
		init_board(void);
		find_all_moves(char, char);
		bool whose_turn();								//return 0 for computer's turn, 1 for player's turn
		bool goal();
		invert_colors(char, int, int);
		copystate(state *);
		copybackstate(state *);
		int heuristic(bool);
		int search();
		int max_value(int, int alpha, int beta);
	 	int min_value(int, int alpha, int beta);
		void makemove(int, char);
		~board(void);
		int search_board(int, int, int, char, char);
		virtual void choice();
		virtual void display_move();

};

inline board::board(void){
	for(int i=0;i<=18;i++){
		for(int j=0;j<=19; j++){
			if(j==18)
				pieces[i][j]='\n';
			else if(j==19)
				pieces[i][j]='\0';
			else if((i==0)&&(j%2==0)&&(j!=0))
				pieces[i][j] =j/2+48;
			else if((j==0)&&(i%2==0)&&(i!=0))
				pieces[i][j] =i/2+48;
			else if((i%2==1)&&(j!=0))
				pieces[i][j]= '-';
			else if((j%2==1)&&(i!=0))
				pieces[i][j]= '|';
			else
				pieces[i][j]= ' ';			
		}
	}
	
	pieces[8][10]='B';
	pieces[10][8]='B';
	pieces[8][8]='W';
	pieces[10][10]='W';
	pieces[17][18]='\0';	

	count[0] = 2;
	count[1] = 2;
	current_player=BLACK;
	player_color =BLACK;
	timer = 10;
	nextmove[0]=nextmove[1]=-1;
	number_legal_moves = 0;
	p1=NULL;
	p2=NULL;
	for(i=0;i<60;i++){
		legal_move_x[i]=0;
		legal_move_y[i]=0;
		h[i]=-INF;
	}

	for(i=0;i<8;i++)
			convert[i]=0;	
	level =1;
	complete =0;
	end_time =0;
	current =0;
}

inline int board::heuristic(bool x)
{
	int eval=0;
	int i;

	find_all_moves(p1, p2);
	eval=M*number_legal_moves + P*count[!player_color];	
	find_all_moves(p2, p1);
	eval-=(M*number_legal_moves + P*count[player_color]);	

	if(pieces[2][2]==p1)
		eval+=CN;
	else if (pieces[2][2]==p2)
		eval-=CN;
	else {
		if(pieces[4][4]==p1)
			eval+=X;
		else if(pieces[4][4]==p2)
			eval-=X;
	}

	if(pieces[2][16]==p1)
		eval+=CN;
	else if (pieces[2][16]==p2)
		eval-=CN;
	else {
		if(pieces[4][14]==p1)
			eval+=X;
		else if(pieces[4][14]==p2)
			eval-=X;
	}

	if(pieces[16][2]==p1)
		eval+=CN;
	else if (pieces[16][2]==p2)
		eval-=(CN-1);
	else {
		if(pieces[14][4]==p1)
			eval+=X;
		else if(pieces[14][4]==p2)
			eval-=X;
	}

	if(pieces[16][16]==p1)
		eval+=CN;
	else if (pieces[16][16]==p2)
		eval-=CN;
	else {
		if(pieces[14][14]==p1)
			eval+=X;
		else if(pieces[14][14]==p2)
			eval-=X;
	}

	for(i=2; i<17; i+=2){
		if(pieces[i][2]==p1)
			eval+=E;
		else if (pieces[i][2]==p2)
			eval-=E;
		if(pieces[2][i]==p1)
			eval+=E;
	    else if	(pieces[2][i]==p2)
			eval-=E;
		if(pieces[i][16]==p1)
			eval+=E;
		else if	(pieces[i][16]==p2)
			eval-=E;
		if(pieces[16][i]==p1)
			eval+=E;
		else if	(pieces[16][i]==p2)
			eval-=E;
	}
	
	//Adding up all the corners: good when computer takes them, bad when player takes them.
	//adding up all the x blocks: bad when the corner is not taken and computer takes them or when both corner and 
	//the x blocks have been taken by the player.  Good when the corner is not taken and the player takes it.
	//adding up all the c blocks: good when the corner is not taken and player takes them.  But bad when computer 
	//takes them before the corners are taken. 
	
	//upper left corner 
/*	if(pieces[2][2]==p1){
		eval+=CN;
		//c-blocks are taken?
		if(pieces[4][2]==p1 || pieces[2][4]==p1)
			eval-=C;
		//x-blocks are taken?
		if(pieces[4][4]==p1)
			eval-=X;

	}
	else if(pieces[2][2]==p2){
		eval-=CN;
		if (pieces[4][2]==p2 || pieces[2][4]==p2)
			eval+=C;
		//x-blocks are taken?
		if(pieces[4][4]==p2)
			eval+=X;
	}
	else{				
		//x-blocks are taken?
		if(pieces[4][4]==p1)
			eval+=X;
		else if (pieces[4][4]==p2)
			eval-=X;
		//c-blocks are taken?
		if(pieces[4][2]==p1 || pieces[2][4]==p1)
			eval+=C;
		if (pieces[4][2]==p2 || pieces[2][4]==p2)
			eval-=C;
		//Tevale edges are taken?
		for(i=6; i<13; i+=2){
			if(pieces[i][2]==p1)
				eval+=E;
			else if(pieces[i][2]==p2)
				eval-=E;
			if(pieces[2][i]==p1)
				eval+=E;
			else if(pieces[2][i]==p2)
				eval-=E;
	
		}	
	}	
	

	//upper right corner
	if(pieces[2][16]==p1){
		eval+=CN;
		if(pieces[2][14]==p1 || pieces[4][16]==p1)
			eval-=C;
		//x-blocks are taken?
		if(pieces[4][14]==p1)
			eval-=X;
	}
	else if(pieces[2][16]==p2){
		eval-=CN;
		if(pieces[2][14]==p2 || pieces[4][16]==p2)
			eval+=C;
		if (pieces[4][14]==p2)
			eval+=X;
	}
	else{
		//x-blocks are taken?
		if(pieces[4][14]==p1)
			eval+=X;
		else if (pieces[4][14]==p2)
			eval-=X;
		//c-blocks are taken?
		if(pieces[2][14]==p1 || pieces[4][16]==p1)
			eval+=C;
		if(pieces[2][14]==p2 || pieces[4][16]==p2)
			eval-=C;
		//The edges are taken?
		for(i=6; i<13; i+=2){
			if(pieces[2][i]==p1)
				eval+=E;
			else if(pieces[2][i]==p2)
				eval-=E;
			if(pieces[i][16]==p1)
				eval+=E;
			else if(pieces[i][16]==p2)
				eval-=E;
		}

	}
		
	//lower left corner
	if(pieces[16][2]==p1){
		eval+=CN;
		if(pieces[14][2]==p1 || pieces[16][4]==p1)
			eval-=C;
		//x-blocks are taken?
		if(pieces[14][4]==p1)
			eval-=X;	
	}
	else if(pieces[16][2]==p2){
		eval-=CN;
		if(pieces[14][2]==p2 || pieces[16][4]==p2)
			eval+=C;
		if (pieces[14][4]==p2)
			eval+=X;
	}
	else{
		//x-blocks are taken?
		if(pieces[14][4]==p1)
			eval+=X;
		else if (pieces[14][4]==p2)
			eval-=X;
		//c-blocks are taken?
		if(pieces[14][2]==p1 || pieces[16][4]==p1)
			eval+=C;
		if(pieces[14][2]==p2 || pieces[16][4]==p2)
			eval-=C;
		//The edges are taken?
		for(i=6; i<13; i+=2){
			if(pieces[16][i]==p1)
				eval+=E;
			else if(pieces[16][i]==p2)
				eval-=E;
			if(pieces[i][2]==p1)
				eval+=E;
			else if(pieces[i][2]==p2)
				eval-=E;
		}

	}

	//lower right corner
	if(pieces[16][16]==p1){
		eval+=CN;
		if(pieces[14][16]==p1 || pieces[16][14]==p1)
			eval-=C;
		//x-blocks are taken?
		if(pieces[14][14]==p1)
			eval-=X;
	}		
	else if(pieces[16][16]==p2){
		eval-=CN;
		if(pieces[14][16]==p2 || pieces[16][14]==p2)
			eval+=C;
		if(pieces[14][14]==p2)
			eval+=X;
	}
	else{
		//x-blocks are taken?
		if(pieces[14][14]==p1)
			eval+=X;
		else if(pieces[14][14]==p2)
			eval-=X;
		//c-blocks are taken?
		if(pieces[14][16]==p1 || pieces[16][14]==p1)
			eval+=C;
		if(pieces[14][16]==p2 || pieces[16][14]==p2)
			eval-=C;
		//The edges are taken?
		for(i=6; i<13; i+=2){
			if(pieces[i][16]==p1)
				eval+=E;
			else if(pieces[i][16]==p2)
				eval-=E;
			if(pieces[16][i]==p1)
				eval+=E;
			else if(pieces[16][i]==p2)
				eval-=E;
		}
	}*/

	return eval;
}

inline void board::makemove(int index, char who)
{
	pieces[legal_move_x[index]][legal_move_y[index]]=who;
	invert_colors(who,legal_move_x[index],legal_move_y[index]);
}

inline board::copystate(state* s)
{
	for(int i=0; i<18; i++){
		for(int j=0; j<19; j++){
			s->pieces[i][j]=pieces[i][j];
		}
	}
	for (i=0; i<2; i++){
		s->count[i]=count[i];
		s->nextmove[i]=nextmove[i];
	}
	for(i=0; i<8; i++)
		s->convert[i]=convert[i];

	s->timer=timer;
	s->current_player=current_player;	
	for(i=0; i<60; i++){
		s->legal_move_x[i]=legal_move_x[i];
		s->legal_move_y[i]=legal_move_y[i];
	}
	s->number_legal_moves=number_legal_moves;
}

inline board::copybackstate(state *s)
{
	for(int i=0; i<18; i++){
		for(int j=0; j<19; j++){
			pieces[i][j]=s->pieces[i][j];
		}
	}
	for (i=0; i<2; i++){
		count[i]=s->count[i];
		nextmove[i]=s->nextmove[i];
	}
	for(i=0; i<8; i++)
		convert[i]=s->convert[i];

	timer=s->timer;
	current_player=s->current_player;	
	for(i=0; i<60; i++){
		legal_move_x[i]=s->legal_move_x[i];
		legal_move_y[i]=s->legal_move_y[i];
	}
	number_legal_moves=s->number_legal_moves;
}
			
inline int board::search(){
	
	int v=0;
	time_t seconds=0;
	int copyv=0;
	int hcopy[60];
	int i =0;
	state b;
	level =1;

	time(&seconds);
	end_time = seconds + timer;

	while(current<end_time){
		copyv=v;
		for(i=0; i<60; i++)
			hcopy[i]=h[i];
		copystate(&b);
		//start alpha beta search 
		v = max_value(level, -INF, INF);
		level++;
	}
	level--;
	if(level > (64-count[0]-count[1]))
		level=(64-count[0]-count[1]);
	for(i=0; i<60; i++)
			h[i]=hcopy[i];
	copybackstate(&b);
	return v;

}

inline int board::max_value(int n, int alpha, int beta){
	
	int v=-INF;
	state copy;
	state succ;
	int temp;

	copystate(&copy);
	find_all_moves(p1, p2);
	copystate(&succ);
	time(&current);

/*****End state ********************************************/
	if(current>end_time)
		return 0;

	if(n==0 || number_legal_moves==0 || (count[0]+count[1])==64)
	{
		return heuristic(0);
	}

	for(int a=0; a<number_legal_moves; a++)
	{
		makemove(a, p1);
		count[!player_color]++;
		current_player=player_color;
		temp=min_value(n-1, alpha, beta);
		v=MAX(v,temp);
		copybackstate(&succ);
		if(n==level)
			h[a]=v;
		if (v>=beta){
			copybackstate(&copy);
			return v;
		}
		alpha=MAX(alpha, v);
	}
	copybackstate(&copy);
	return v;
}

inline int board::min_value(int n, int alpha, int beta){

	int v=INF, temp;
	state copy, succ;
	time(&current);
	ofstream f;

	copystate(&copy);
	find_all_moves(p2, p1);
	copystate(&succ);

/*****End state ********************************************/
	if(current>end_time)
		return 0;

	if(n==0 || number_legal_moves==0 || (count[0]+count[1])==64)
	{
		return heuristic(1);
	}

	for(int a=0; a<number_legal_moves; a++)
	{
		makemove(a,p2);
		count[player_color]++;
		current_player=!player_color;
		temp = max_value(n-1, alpha, beta);
		v=MIN(v, temp);
		copybackstate(&succ);
		if (v<=alpha){
			copybackstate(&copy);
			return v;
		}
		beta=MIN(beta, v);
		
	}
	copybackstate(&copy);

	return v;
};

inline board::init_board()
{
		char a;
		time_t t=0;
		bool s=1;

		system("cls"); 
		cout<<&pieces[0][0];
		cout<<endl<<"Number of White Pieces: "<<count[1]<<endl
			<<"Number of Black Pieces: "<<count[0]<<endl;
		if(current_player==BLACK)
			cout<<"It is Black's turn."<<endl;
		else
			cout<<"It is White's turn."<<endl;
		while(s){
			cout<<"Please Enter Alpha Beta Search Time: (<15 s)"<<endl;
			cin>>t;
			if(t>0 && t<15){
				timer =t;
				s=0;
				break;
			}
			else{
				cout<<"Invalid! Try again."<<endl;
				cin.clear();
				cin.ignore(INT_MAX,'\n');
			}
		}

		cout<<"Do you want to go first? <y/n>"<<endl;
		cin>>a;
		
		if(a=='y'){
			player_color = BLACK;
			p1='W';						//computer is white
			p2='B';						//player is black
		}
		else{
			player_color = WHITE;	
			p1='B';						//computer is black
			p2='W';						//player is white
			cout<<"Please Wait For Computer Move..."<<endl;
		}	
		
}			

inline board::invert_colors(char who, int x, int y){
	
		int i;
		char p11, p22;
		p11=who;
		if(who==p1)
			p22=p2;
		else
			p22=p1;

			convert[0]=search_board(UP, x, y, p11, p22);
			convert[1]=search_board(DOWN, x, y, p11, p22);
			convert[2]=search_board(LEFT, x, y, p11, p22);
			convert[3]=search_board(RIGHT, x, y, p11, p22);
			convert[4]=search_board(DIAG_45, x, y, p11, p22);
			convert[5]=search_board(DIAG_135, x, y, p11, p22);
			convert[6]=search_board(DIAG_225, x, y, p11, p22);
			convert[7]=search_board(DIAG_315, x, y, p11, p22);
		
		for(i=1; i<convert[0]; i++){
			pieces[x-2*i][y]=who;
			count[current_player]++;
			count[!current_player]--;
		}
		for(i=1; i<convert[1]; i++){
			pieces[x+2*i][y]=who;
			count[current_player]++;
			count[!current_player]--;
		}
		for(i=1; i<convert[2]; i++){
			pieces[x][y-2*i]=who;
			count[current_player]++;
			count[!current_player]--;
		}
		for(i=1; i<convert[3]; i++){
			pieces[x][y+2*i]=who;
			count[current_player]++;
			count[!current_player]--;
		}
		for(i=1; i<convert[4]; i++){
			pieces[x-2*i][y+2*i]=who;
			count[current_player]++;
			count[!current_player]--;
		}
		for(i=1; i<convert[5]; i++){
			pieces[x-2*i][y-2*i]=who;
			count[current_player]++;
			count[!current_player]--;
		}
		for(i=1; i<convert[6]; i++){
			pieces[x+2*i][y-2*i]=who;
			count[current_player]++;
			count[!current_player]--;
		}
		for(i=1; i<convert[7]; i++){
			pieces[x+2*i][y+2*i]=who;
			count[current_player]++;
			count[!current_player]--;
		}
}

inline bool board::whose_turn(){
	if(current_player==player_color)
		return 1;
	else 
		return 0;
}

inline bool board::goal(){
	if((count[0]+count[1])==64){
		if(count[player_color]>count[!player_color])
			cout<<"You Won!"<<endl;
		else if (count[player_color]<count[!player_color])
			cout<<"You Lost!"<<endl;
		else 
			cout<<"It is a draw!"<<endl;
		return 1;
	}
	else 
		return 0;
}


inline board::find_all_moves(char p11, char p22){
	int i,j;
	number_legal_moves = 0;

	for(i=0;i<60; i++){
		legal_move_x[i] = 0;
		legal_move_y[i] = 0;
	}

	for(i=2; i<17; i+=2){
		for(j=2;j<17;j+=2){
			if(pieces[i][j]==' '){			// if the space is empty
					search_board(UP, i, j, p11, p22);
					search_board(DOWN, i, j, p11, p22);
					search_board(LEFT, i, j, p11, p22);
					search_board(RIGHT, i, j, p11, p22);
					search_board(DIAG_45, i, j, p11, p22);
					search_board(DIAG_135, i, j, p11, p22);
					search_board(DIAG_225, i, j, p11, p22);
					search_board(DIAG_315, i, j, p11, p22);
			}
		}		
	}	

//	cout<<"moves: ";
//	for(i=0;i<number_legal_moves; i++){
//		cout<<" "<<legal_move_x[i]<<"," <<legal_move_y[i]<<" ";
//	}
//	cout<<endl;

}

inline int board::search_board(int direction, int x, int y, char p11, char p22){

	int i = x;
	int j = y;
	bool p1_found = 0;
	bool p2_found = 0;
	bool blank_found =0;
	int	 count=0;
	bool redundant=0;

	switch(direction){
			case UP:
				while(i>3 && i<17 && j>1 && j<17 && !p1_found && !blank_found){
					i-=2;
					count++;
					if(pieces[i][j]==p22)
						p2_found = 1;
					else if(pieces[i][j]==p11)
						p1_found = 1;
					else 
						blank_found = 1;
				}
				break;

			case DOWN:
				while(i>1 && i<15 && j>1 && j<17 && !p1_found && !blank_found){
					i+=2;
					count++;
					if(pieces[i][j]==p22)
						p2_found = 1;
					else if(pieces[i][j]==p11)
						p1_found = 1;
					else 
						blank_found = 1;
				}
				break;

			case LEFT:
				while(i>1 && i<17 && j>3 && j<17 && !p1_found && !blank_found){
					j-=2;
					count++;
					if(pieces[i][j]==p22)
						p2_found = 1;
					else if(pieces[i][j]==p11)
						p1_found = 1;
					else 
						blank_found = 1;
				}
				break;

			case RIGHT:
				while(i>1 && i<17 && j>1 && j<15 && !p1_found && !blank_found){
					j+=2;
					count++;
					if(pieces[i][j]==p22)
						p2_found = 1;
					else if(pieces[i][j]==p11)
						p1_found = 1;
					else 
						blank_found = 1;
				}
				break;

			case DIAG_45:
				while(i>3 && i<17 && j>1 && j<15 && !p1_found && !blank_found){
					i-=2;
					j+=2;
					count++;
					if(pieces[i][j]==p22)
						p2_found = 1;
					else if(pieces[i][j]==p11)
						p1_found = 1;
					else 
						blank_found = 1;
				}
				break;

			case DIAG_135:
				while(i>3 && i<17 && j>3 && j<17 && !p1_found && !blank_found){
					i-=2;
					j-=2;
					count++;
					if(pieces[i][j]==p22)
						p2_found = 1;
					else if(pieces[i][j]==p11)
						p1_found = 1;
					else 
						blank_found = 1;
				}
				break;

			case DIAG_225:
				while(i>1 && i<15 && j>3 && j<17 && !p1_found && !blank_found){
					i+=2;
					j-=2;
					count++;
					if(pieces[i][j]==p22)
						p2_found = 1;
					else if(pieces[i][j]==p11)
						p1_found = 1;
					else 
						blank_found = 1;
				}
				break;

			case DIAG_315:
				while(i>1 && i<15 && j>1 && j<15 && !p1_found && !blank_found){
					i+=2;
					j+=2;
					count++;
					if(pieces[i][j]==p22)
						p2_found = 1;
					else if(pieces[i][j]==p11)
						p1_found = 1;
					else 
						blank_found = 1;
				}
				break;

			default:
					break;
		
	}

	if(p1_found && p2_found){
	
		for(i=0; i<number_legal_moves; i++){
			
			if((legal_move_x[i]==x) && (legal_move_y[i]==y))
				redundant =1;
		}
		if(redundant==0){
				legal_move_x[number_legal_moves]=x;
				legal_move_y[number_legal_moves]=y;
				number_legal_moves++;
		}
	}
			
	else 
		count = 0;

	return count;
}	

inline void board::choice(){}
inline void board::display_move(){}
inline board::~board()
{
}


#endif
