#ifndef _gameboard_player_H
#define _gameboard_player_H
#include"gameboard.h"

class gameboard_player : public board
{
	public:
		~gameboard_player();
		void choice();
		void display_move();
};

void gameboard_player::choice(){
		int a=0,b=0,i=0;
		bool legal=0;

		find_all_moves(p2, p1);
		for(i=0;i<number_legal_moves;i++){
			pieces[legal_move_x[i]][legal_move_y[i]]='*';
		}
//		system("cls");
		
		cout<<"Player's choice"<<endl;

		cout<<&pieces[0][0];
		cout<<endl<<"Number of White Pieces: "<<count[1]<<endl
			<<"Number of Black Pieces: "<<count[0]<<endl;
		if(player_color==BLACK)
			cout<<"It is Black's turn."<<endl;
		else
			cout<<"It is White's turn."<<endl;
		cout<<"Level of the Last alpha beta search: "<<level<<endl;
		if(number_legal_moves ==0){
			cout<<"no legal moves, you must pass! <press enter>"<<endl;
			getchar();
		}
		else{
			while(!legal){
				cout<<"Enter your next move <xy>: "<<endl;
				cin>>a;
				b=a%10;
				a=a/10;
				if(a>0 && a<9 && b>0 && b<9){				
					for(i=0; i<number_legal_moves; i++){
						if(a==(legal_move_x[i]/2) && b==(legal_move_y[i]/2)){
							legal=1;
							break;
						}
						else 
							legal=0;
					}
				}
				else 
					legal = 0;
				if(!legal){
					cout<<"Not a legal move"<<endl;
					cin.clear();
					cin.ignore(INT_MAX,'\n');
				}
			}
		}
		nextmove[0]=a*2;
		nextmove[1]=b*2;
}

void gameboard_player::display_move(){
		int a=0,b=0,i=0;
		if(number_legal_moves !=0){
			for(i=0;i<number_legal_moves;i++){
				pieces[legal_move_x[i]][legal_move_y[i]]=' ';
			}
			pieces[nextmove[0]][nextmove[1]]=p2;
			count[current_player]++;
			invert_colors(p2, nextmove[0], nextmove[1]);
		}
		
//		system("cls"); 
		cout<<"Player's display"<<endl;
		cout<<&pieces[0][0]<<endl;
		cout<<"Alpha Beta Search Level: "<<level<<endl; 
		cout<<"Number of White Pieces: "<<count[1]<<endl
			<<"Number of Black Pieces: "<<count[0]<<endl;
		cout<<"Wait for the computer's move..."<<endl;
		current_player=!current_player;

}

gameboard_player::~gameboard_player(){}


#endif
