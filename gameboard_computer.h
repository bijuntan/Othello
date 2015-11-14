#ifndef _gameboard_computer_H
#define _gameboard_computer_H
#include"gameboard.h"


class gameboard_computer : public board
{
	public:
		~gameboard_computer();
		void choice();
		void display_move();
};

void gameboard_computer::choice(){
		int y , v;
		int i=0;
		int t[60];
		int equal_moves=0;
		int mx[60], my[60];
		
		find_all_moves(p1, p2);
		for(i=0;i<number_legal_moves; i++){
			pieces[legal_move_x[i]][legal_move_y[i]]='*';
		}
		cout<<&pieces[0][0]<<endl;
		for(i=0;i<number_legal_moves; i++){
			pieces[legal_move_x[i]][legal_move_y[i]]=' ';
		}
		if(number_legal_moves !=0){
			v=search();
			for(i=0; i<number_legal_moves; i++){
				if(h[i]==v){
					mx[equal_moves]=legal_move_x[i];
					my[equal_moves]=legal_move_y[i];
					equal_moves++;
				}
			}
			// for moves that has equal heuristics, randomize the choice
			srand( (unsigned)time( NULL ) );
			y=rand()%equal_moves;

			nextmove[0]=mx[y];
			nextmove[1]=my[y];
			pieces[nextmove[0]][nextmove[1]]=p1;
			count[current_player]++;
			invert_colors(p1, nextmove[0], nextmove[1]);
		}
		else{
			cout<<"Computer has no moves."<<endl;
		}
}

void gameboard_computer::display_move(){
		int i=0, j=0;
		int a=0,b=0;

//		system("cls"); 
		cout<<"computer display"<<endl;
		cout<<&pieces[0][0];
		cout<<endl<<"Number of White Pieces: "<<count[1]<<endl
			<<"Number of Black Pieces: "<<count[0]<<endl;
		current_player=!current_player;
}

gameboard_computer::~gameboard_computer(){}

#endif
