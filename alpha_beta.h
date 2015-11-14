#ifndef _alpha_beta_H
#define _alpha_beta_H

#include "gameboard_turn.h"
#define  INF 100000000
#define  max(a,b) (((a)>(b))?(a):(b)) 
#define  min(a,b) (((a)<(b))?(a):(b)) 

class alpha_beta_search : board {
		
	struct state{
		char pieces2[18][19];
		int  legal_move_x2[60], legal_move_y2[60];	
		int  number_legal_moves2;	
	};

	public:
		alpha_beta_search(void);
		~alpha_beta_search(void);
		int search(int);
		int max_value(int, state*, int alpha, int beta);
		int min_value(int, state*, int alpha, int beta);
		void successor(state *);
		int heuristic(state *);
		void copystate(state * s1, state *s2);
};

alpha_beta_search::alpha_beta_search(){}

/* Find all possible moves 
 * copy the number of moves 
 */

void alpha_beta_search::successor(state* p){
		find_all_moves(p->pieces2);
		for(int i=0; i<60; i++){
			p->legal_move_x2[i] = legal_move_x[i];
			p->legal_move_y2[i] = legal_move_y[i];
		}

		p->number_legal_moves2 = number_legal_moves;
}

int alpha_beta_search::heuristic(state* p){}

alpha_beta_search::~alpha_beta_search(){}

void alpha_beta_search::copystate(state * s1, state *s2){
	
	for(int i=0;i<18;i++){
		for(int j=0;j<19; j++){
			s1->pieces2[i][j]=s2->pieces2[i][j];
		}
	}

	for(i=0; i<60; i++){
	s1->legal_move_x2[i] = s2->legal_move_x2[i];
	s1->legal_move_y2[i] = s2->legal_move_y2[i];
	}

	s1->number_legal_moves2 = s2->number_legal_moves2;
}
	
// returns the index of the best move
// this is iterative deepening
int alpha_beta_search::search(int n){
	
	int v=0;
	state copy;

// pass the copy of the board to the max_value function
	
	v = max_value(n,&copy, -INF, INF);

//restore the game state

	return v;
}

int alpha_beta_search::max_value(int n, state* p, int alpha, int beta){
	
	int v=-INF;
	state copy;
	
	successor(&copy);

/*****End state ********************************************/
	if(n==0 || copy.number_legal_moves2==0)
	{
		return heuristic(&copy);
	}

	for(int a=0; a<copy.number_legal_moves2; a++)
	{
		v=max(v, min_value(n-1,&max_child[a], alpha, beta));
		if (v>=beta){
			return v;
		}
		alpha=max(alpha, v);
	}
	
	return v;
}

int alpha_beta_search::min_value(int n, state* p, int alpha, int beta){

	int v=INF;
	state copy;
	successor(&copy);

/*****End state ********************************************/
	if(n==0 || copy.number_legal_moves2==0)
	{
		return heuristic(&copy);
	}

	for(int a=0; a<2; a++)
	{
		v=min(v, max_value(n-1, min_child[a], alpha, beta));
		if (v<=alpha)
			return v;
		beta=min(beta, v);
		
	}
	return v;
}

#endif
