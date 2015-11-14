#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#define  INF 1000
#define  max(a,b) (((a)>(b))?(a):(b)) 
#define  min(a,b) (((a)<(b))?(a):(b)) 


struct state{
		int a;
		struct state* child[2];
};

class alpha_beta_search {

	protected:
		state s[15];
		state action;

	public:
		alpha_beta_search(void);
		~alpha_beta_search(void);
		int search(int);
		int max_value(int, state*, int alpha, int beta);
		int min_value(int, state*, int alpha, int beta);
		void successor(state *);
		int heuristic(state *);
};

alpha_beta_search::alpha_beta_search(){	

	s[0].a = 0;
	s[0].child[1] = &s[2];
	s[0].child[0] = &s[1];
	s[1].a = 1;
	s[1].child[1] = &s[3];
	s[1].child[0] = &s[4];
	s[2].a = 2;
	s[2].child[1] = &s[12];
	s[2].child[0] = &s[9];
	s[3].a = 3;
	s[3].child[1] = &s[5];
	s[3].child[0] = &s[6];
	s[4].a = 4;
	s[4].child[1] = &s[7];
	s[4].child[0] = &s[8];
	s[5].a = 5;
	s[5].child[1] = NULL;
	s[5].child[0] = NULL;
	s[6].a = 6;
	s[6].child[0] = NULL;
	s[6].child[1] = NULL;
	s[7].a = 7;
	s[7].child[0] = NULL;
	s[7].child[1] = NULL;
	s[8].a = 8;
	s[8].child[0] = NULL;
	s[8].child[1] = NULL;
	s[9].a = 9;
	s[9].child[0] = &s[10];
	s[9].child[1] = &s[11];
	s[10].a = 10;
	s[10].child[0] = NULL;
	s[10].child[1] = NULL;
	s[11].a = 11;
	s[11].child[0] = NULL;
	s[11].child[1] = NULL;
	s[12].a = 12;
	s[12].child[0] = &s[13];
	s[12].child[1] = &s[14];
	s[13].a = 13;
	s[13].child[0] = NULL;
	s[13].child[1] = NULL;
	s[14].a = 14;
	s[14].child[0] = NULL;
	s[14].child[1] = NULL;

}

void alpha_beta_search::successor(state* p){

	action.a =0;
	action.child[0]=p->child[0];	
	action.child[1]=p->child[1];		

}

int alpha_beta_search::heuristic(state* p){
		return p->a;
}

alpha_beta_search::~alpha_beta_search(){}

// returns the index of the best move
// this is iterative deepening
int alpha_beta_search::search(int n){
	
	int v=0;
	int moves=0;

	v = max_value(n,&s[0], -INF, INF);

	//find the state v corresponding to the returned value;
	return v;
}

int alpha_beta_search::max_value(int n, state* p, int alpha, int beta){
	
	int v=-INF;
	state max_child;

	if(n==0){
		return heuristic(p);
	}
//	successor(p);
	max_child.a=0;
	max_child.child[0]= p->child[0];
	max_child.child[1]= p->child[1];

	for(int a=0; a<2; a++)
	{
		if(max_child.child[a]!=NULL){
			v=max(v, min_value(n-1,max_child.child[a], alpha, beta));
			if (v>=beta){
				return v;
			}
			alpha=max(alpha, v);
		}
		else;
	}
	return v;
}

int alpha_beta_search::min_value(int n, state* p, int alpha, int beta){

	int v=INF;
	state min_child;

	if(n==0){
		return heuristic(p);
	}
	//successor(p);

	min_child.child[0]=p->child[0];
	min_child.child[1]=p->child[1];
	
	for(int a=0; a<2; a++)
	{
		if(min_child.child[a]!=NULL){
			v=min(v, max_value(n-1, min_child.child[a], alpha, beta));
			if (v<=alpha)
				return v;
			beta=min(beta, v);
		}
		else;
	}
	return v;
}

void main(){
	int result;
	int i;
	int n=3;
	alpha_beta_search game;
	
	for(i=1;i<=n; i++){
	result = game.search(i);
	cout<<"result = "<<result<<endl;
	}

}
	
	
