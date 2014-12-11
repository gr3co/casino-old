#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <time.h>

typedef struct crd{
	int value;
	char suit;
} *card;

typedef card* deck;

typedef struct plr{
	int money;
	deck* hands;
	int* num_cards;
	int num_hands;
	int player_number;
	int bet;
	int active;
} *player; 

typedef struct gm{
	player* players;
	deck cards;
	int num_players;
	int num_decks;
	int pot;
} *game;

card new_card(int,int);
deck new_deck_ordered();
deck new_deck_shuffled();
void shuffle(deck);
void print_card(card);
void print_deck(deck,int);
card draw(deck,int);
int deck_empty(deck,int);
void deal(card (*draw)(deck),deck,player*,int,int);
player new_player(int,int,int,int);
void print_hand(player);
void print(int);
int wager(player,int,game);
void error();
void player_data(player);
void free_players(player*);
void free_deck(deck);
char tolower(char);