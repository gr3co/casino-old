#include "barebones.h"

void free_deck(deck d){
	int i;
	for (i=0; d[i]; i++)
		free(d[i]);
	free(d);
}

void free_players(player* p){
	int i;
	int j;
	for (i=0; p[i]; i++){
		for (j=0; p[i]->hands[j]; j++)
			free_deck(p[i]->hands[j]);
		free(p[i]->hands);
		free(p[i]);
	}
	free(p);
}

void print(int x){
	printf("%d\n",x);
}

card new_card(int s, int v){
	card result = (card)malloc(sizeof(struct crd));
	result->value = v;
	switch (s){
		case 0:
		result->suit='S';
		break;
		case 1:
		result->suit='C';
		break; 
		case 2:
		result->suit='D';
		break;
		case 3:
		result->suit='H';
		break;
	}
	return result;
}

deck new_deck_ordered(){
	deck result;
	result = (deck)malloc(52 * sizeof(struct crd*));
	short i;
	short j;
	short index;
	index = 0;
	for (i=0;i<4;i++)
		for(j=1;j<14;j++){
			result[index] = new_card(i,j);
			index++;
		}
		return result;
	}

	deck new_deck_shuffled(){
		deck result;
		result = new_deck_ordered();
		shuffle(result);
		return result;
	}

	void shuffle(deck d){
	assert(d[0]); //deck must be full
	unsigned int iseed = (unsigned int)time(NULL);
	srand(iseed);
	int i;
	for (i = 0; i < 999999; i++){
		int j;
		int k;
		j = rand() % 52;
		k = rand() % 52;
		while (j == k)
			k = rand() % 52;
		card temp = d[k];
		d[k] = d[j];
		d[j] = temp;
	}
}

void print_card(card c){
	switch (c->value){
		case 1:
		printf("A\t%c\n", c->suit);
		break;
		case 11:
		printf("J\t%c\n", c->suit);
		break;
		case 12:
		printf("Q\t%c\n", c->suit);
		break;
		case 13:
		printf("K\t%c\n", c->suit);
		break;
		default:
		printf("%d\t%c\n", c->value, c->suit);
	}
}

void print_deck(deck d, int size){
	int i;
	for (i = 0; i < size; i++){
		if (d[i] != NULL)
			print_card(d[i]);
	}
}

int deck_empty(deck d, int size){
	int result;
	result = 1;
	int i;
	for (i = 0; i < size; i++){
		if (d[i])
			result = 0;
	}
	return result;
}

card draw(deck d,int size){
	if (deck_empty(d,size)){
		printf("Deck Empty.\n");
		return new_card(0,0);
	}

	int i;
	card result;
	for (i = 0; !d[i]; i++){
	}
	result = d[i];
	d[i] = NULL;
	return result;
}

void error(){
	printf("\nError! Terminating.....\n\n");
	exit(0);
}

void deal(card (*df)(deck), deck d, player* p, int num_players, int num){
	int i;
	int j;
	for (i = 0; i < num; i++){
		for (j = 0; j < num_players; j++){
			p[j]->hands[0][p[j]->num_cards[0]] = (*df)(d);
			p[j]->num_cards[0]++;	
		}
	}
}

void print_hand(player p){
	if (p->num_hands == 1){
		printf("Player %d's Hand\n", p->player_number);
		int i;
		for (i = 0; i < p->num_cards[0]; i++)
			print_card(p->hands[0][i]);
	}
	else{
		int i;
		int j;
		printf("Player %d's Hands\n", p->player_number);
		for (i = 0; i < p->num_hands; i++){
			printf("Hand %d:\n", i + 1);
			for (j = 0; j < p->num_cards[i]; j++)
				print_card(p->hands[i][j]);
		}
	}
}

int wager(player p, int num, game g){
	assert(p!=NULL);
	//assert(p->money != 0);
	if (num > p->money || num <= 0){
		printf("Cannot bet %d\n", num);
		return 0;
	}
	p->money = p->money - num;
	p->bet = p->bet + num;
	g->pot = g->pot + num;
	return 1;
}

player new_player(int cash, int num, int numhands, int handsize){
	int i;
	player result = (player)malloc(sizeof(struct plr));
	result->money = cash;
	result->hands = (deck*)malloc(numhands * sizeof(deck));
	result->num_cards = (int*)malloc(numhands * sizeof(int));
	for (i = 0; i < numhands; i++){
		result->hands[i] = (deck)malloc(handsize * sizeof(struct crd*));
		result->num_cards[i] = 0;
	}
	result->num_hands = 1;
	result->player_number = num;
	result->bet = 0;
	result->active = 1;
	return result;
}

void player_data(player p){
	print_hand(p);
	printf("Money left: $%d\n", p->money);
	printf("Current bet: $%d\n", p->bet);
}

char tolower(char p){
	return p>='A'&&p<='Z'?p|0x60:p;
}

