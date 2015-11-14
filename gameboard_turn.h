#ifndef _gameboard_turn_H
#define _gameboard_turn_H
#include "gameboard_computer.h"
#include "gameboard_player.h"

class gameboard_turn {
	
	static gameboard_player   player;
	static gameboard_computer computer;

	public:
		gameboard_turn();
		~gameboard_turn();
		board* get_player(void);
};

gameboard_turn::gameboard_turn(){}
gameboard_turn::~gameboard_turn(){}
board* gameboard_turn::get_player(){
		if(computer.whose_turn()){
			return &player;
		}
		else{
			return &computer;
		}

}

#endif
