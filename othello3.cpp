#include "gameboard_turn.h"
#include "fstream.h"

char board::pieces[18][19];
int	 board::count[2];
time_t  board::timer;
bool board::current_player;							//0->black's turn  1->white's turn
bool board::player_color;							//the color of the user: 0->black, 1->white
int  board::nextmove[2];							//x,y coordinate of the user move
int  board::legal_move_x[60];
int  board::legal_move_y[60];						//points to array of all possible legal moves
int  board::h[60];
int  board::number_legal_moves;						//the total number of legal moves
char board::p1;
char board::p2;	
int	 board::convert[8];
int  board::level;
bool board::complete;
time_t board::end_time;
time_t board::current;

gameboard_player gameboard_turn::player;
gameboard_computer gameboard_turn::computer;

void main()
{
	int end=0;
	gameboard_turn othello;


	(othello.get_player())->init_board();

	while(!end){
	(othello.get_player())->choice();
	(othello.get_player())->display_move();
	end= (othello.get_player())->goal();
	}

	cout<<"game over"<<endl;
}
