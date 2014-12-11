#include "barebones.h"

int main(){
	deck d = new_deck_shuffled();
	player *p = (player*)malloc(2* sizeof(player));
	p[0] = new_player(100, 1, 10);
	p[1] = new_player(100, 2, 10);
	deal(d,p,2,10);
	print_hand(p[0]);
	print_hand(p[1]);
}