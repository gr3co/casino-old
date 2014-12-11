#include "barebones.h"

game new_game_bj();
int add_player_bj(game);
card draw_blackjack(deck);
int sum_cards(deck,int);
void deal_blackjack(game);
int deal_player(game,player,int);
void print_sum(player);
void print_sums_bj(game);
int move(game,player);
void play(game);
void end_game(game);
void payday(game);
void clear_hands(game);
char* split(game,player,int);

int main(){
  game bjgame = new_game_bj();
  add_player_bj(bjgame);
  play(bjgame);
  end_game(bjgame);
}

void end_game(game g){
  printf("\nAll players either ran out of money or quit!\n");
  printf("Thanks for playing Greco's Blackjack!\n\n");
  free_players(g->players);
  free_deck(g->cards);
  free(g);
  printf("\n\n\nPress [enter] to exit...\n");
  getchar();
  printf("\033[2J\033[1;1H");
}

game new_game_bj(){
  game blackjack = (game)malloc(sizeof(struct gm));
  blackjack->players = (player*)malloc(100 * sizeof(player));
  blackjack->num_players = 0;
  blackjack->pot = 0;
  deck *decks = (deck*)malloc(5 * sizeof(deck));
  int i;
  int j;
  for (i = 0; i < 5; i++){
    decks[i] = new_deck_shuffled();
  }
  blackjack->cards = (deck)malloc(52 * 5 * sizeof(card));
  int index;
  index = 0;
  for (i = 0; i < 5; i++){
    for (j = 0; j < 52; j++){
      blackjack->cards[index] = decks[i][j];
      index++;
    } 
  }
  free(decks);
  printf("\033[2J\033[1;1H");
    printf("\n===================================\n");
    printf("Welcome to Greco's Blackjack v1.35!\n");
    printf("===================================\n\n");
    printf("This game has 5 decks. Each player starts with $1000.\n");
    printf("The goal of this game is to get as close to 21\n");
    printf("as possible without going over. You win if you are\n");
    printf("closer than the dealer. Blackjacks return 50%% bonus.\n");
    printf("Aces count as 1 or 11; face cards count as 10.\n");
    printf("To quit, type \'quit\' or \'q\' for your wager.\n");
    printf("\n\n==UPDATE LOG==\n");
    printf("v1.3\n");
    printf("* More aesthetic upgrades.\n");
    printf("* Splitting is now supported.\n");
    printf("v1.2\n");
    printf("* Bugfixes and aesthetic upgrades.\n");
    printf("* Doubling-down is now supported.\n");
    printf("v1.1\n");
    printf("* Various bugfixes in wagering system.\n");
    printf("* Support for up to 99 players pass-and-play.\n");
    return blackjack;
  }

  int add_player_bj(game g){
    if (g->num_players >= 100){
      perror("Cannot add player.\n");
      return 0;
    }
    g->players[g->num_players] = new_player(1000,g->num_players,10,20);
  //player_data(g->players[g->num_players]);
    g->num_players++;
    return 1;
  }


  void deal_blackjack(game g){
  //printf("\nDealing 2 cards to each player...\n");
    deal(&draw_blackjack, g->cards, g->players, g->num_players, 2);
  }

  int sum_cards(deck d, int num){
    int accum;
    int aces;
    int i;
    aces = 0;
    accum = 0;
    for (i = 0; i < num; i++){
      switch(d[i]->value){
        case 1:
        accum += 11;
        aces++;
        break;
        case 11:
        accum += 10;
        break;
        case 12:
        accum += 10;
        break;
        case 13:
        accum += 10;
        break;
        default:
        accum += d[i]->value;
        break;

      }
    }
    for (; aces > 0 && accum > 21; aces--)
      accum -= 10;
    return accum;
  }

  int deal_player(game g, player p, int which_hand){
    if (p->money == 0 & p->bet == 0 || p->active == 0)
      return 0;
    p->hands[which_hand][p->num_cards[which_hand]] = draw_blackjack(g->cards);
    p->num_cards[which_hand]++;	
    if (sum_cards(p->hands[which_hand], p->num_cards[which_hand]) > 21)
      return 0;
    return 1;
  }

  void print_sum(player p){
    int sum;
    sum = sum_cards(p->hands[0], p->num_cards[0]);
    if (p->player_number == 0){
      if (sum == 21 && p->num_cards[0] == 2)
        printf("Dealer:\t\tBLACKJACK\n");
      else if (sum > 21)
        printf("Dealer:\t\t%d (OVER)\n", sum);
      else printf("Dealer:\t\t%d\n", sum);
    }
    else{
      if (p->num_hands == 1){
        if (sum == 21 && p->num_cards[0] == 2)
         printf("Player %d:\tBLACKJACK\n", p->player_number);
       else if (sum > 21)
         printf("Player %d:\t%d (OVER)\n", p->player_number, sum);
       else printf("Player %d:\t%d\n", p->player_number, sum);
     }
     else{
      printf("Player %d:\n", p->player_number);
      int which_hand;
      for (which_hand = 0; which_hand < p->num_hands; which_hand++){
       sum = sum_cards(p->hands[0], p->num_cards[0]);
       if (sum == 21 && p->num_cards[which_hand] == 2)
         printf("   Hand %d:\tBLACKJACK\n", which_hand + 1);
       else if (sum > 21)
         printf("   Hand %d:\t%d (OVER)\n", which_hand + 1, sum);
       else printf("   Hand %d:\t%d\n", which_hand + 1, sum);
     }
   }
 }
}

card draw_blackjack(deck d){
  if (deck_empty(d,52*5)){
    printf("Decks empty. Reshuffling!\n");
    deck *decks = (deck*)malloc(5 * sizeof(deck));
    int i;
    int j;
    for (i = 0; i < 5; i++){
      decks[i] = new_deck_shuffled();
    }
    int index;
    index = 0;
    for (i = 0; i < 5; i++){
      for (j = 0; j < 52; j++){
       d[index] = decks[i][j];
       index++;
     } 
   }
   free(decks);
 }

 return draw(d,5*52);
}

void print_sums_bj(game g){
  printf("\n");
  int i;
  for (i = 0; i < g->num_players; i++){
    if (g->players[i]->active)
      print_sum(g->players[i]);
    printf("\n");
  }
}


void play(game g){
  char* charbuffer = (char*)malloc(32);
  int intbuffer;
  int i;
  int j;
  int active;
  printf("\nPress [enter] to continue...");
  getchar();
  printf("\033[2J\033[1;1H");
    printf("\nHow many players?\nPlease enter a number (1 to 99):  ");
    fgets(charbuffer, sizeof charbuffer, stdin);
    intbuffer = atoi(charbuffer);
    if (intbuffer == 0 || intbuffer >= 100){
      error();
    }
    for (i = 0; i < 32; i++)
      charbuffer[i] = '\0';

    for (; intbuffer > 0; intbuffer--)
      add_player_bj(g);

    printf("\n");
    while (1){

      active = 0;
      for (i = 1; i < g->num_players; i++)
        if (g->players[i]->active)
         active++;

       if (active == 0)
        return;

      for (i = 1; i < g->num_players; i++){
        if (g->players[i]->money == 0){
         g->players[i]->active = 0;
         continue;
       }

       printf("\033[2J\033[1;1H");

        printf("\nPlayer %d you have $%d.\n", i, g->players[i]->money);
        printf("Please enter a wager: ");
        fgets(charbuffer, sizeof charbuffer, stdin);
        for(j = 0; charbuffer[j]; j++)
         charbuffer[j] = tolower(charbuffer[j]);
       if (!strncmp(charbuffer,"quit",4) || !strncmp(charbuffer,"q",1)){
         g->players[i]->money = 0;
         g->players[i]->active = 0;
         continue;
       }
       intbuffer = atoi(charbuffer);
       for (j = 0; j < 32; j++)
         charbuffer[j] = '\0';
       while (wager(g->players[i],intbuffer,g) == 0){
         printf("\nPlayer %d you have $%d.\n", i, g->players[i]->money);
         printf("Please enter a wager: ");
         fgets(charbuffer, sizeof charbuffer, stdin);
         for(j = 0; charbuffer[j]; j++)
           charbuffer[j] = tolower(charbuffer[j]);
         if (!strncmp(charbuffer,"quit",4) || !strncmp(charbuffer,"q",1)){
           g->players[i]->active = 0;
           g->players[i]->money = 0;
           break;
         }
         intbuffer = atoi(charbuffer);
         for (j = 0; j < 32; j++)
           charbuffer[j] = '\0';
       }
     }

     deal_blackjack(g);

     for (i = 1; i < g->num_players; i++)
      move(g,g->players[i]);

    while (sum_cards(g->players[0]->hands[0], g->players[0]->num_cards[0]) < 17)
      deal_player(g, g->players[0],0);

    printf("\033[2J\033[1;1H");
      print_sums_bj(g);
      payday(g);
      clear_hands(g);

      active = 0;
      for (i = 1; i < g->num_players; i++)
        if (g->players[i]->active)
         active++;

       if (active == 0)
        return;

      for (i = 1; i < g->num_players; i++){
        if (g->players[i]->money == 0){
         g->players[i]->active = 0;
         continue;
       }
     }

     char* s = (char*)malloc(1);
     printf("Press [enter] to continue...");
     getchar();
     printf("\033[2J\033[1;1H");

   }


   free(charbuffer);

 }

 void payday(game g){
  int i;
  if (sum_cards(g->players[0]->hands[0], g->players[0]->num_cards[0]) > 21)
    for (i = 1; i < g->num_players; i++){
      int j;
      if (g->players[i]->active == 0)
       continue;
     for (j = 0; j < g->players[i]->num_hands; j++){
       if (sum_cards(g->players[i]->hands[j], g->players[i]->num_cards[j]) <= 21
         && g->players[i]->num_cards[j] > 2){
         printf("Player %d beat dealer!\n", i);
       printf("Player %d wins $%d\n", i, (g->players[i]->bet) 
         / g->players[i]->num_hands);
       g->players[i]->money += (2 * g->players[i]->bet) / g->players[i]->num_hands;
     }
     else if (sum_cards(g->players[i]->hands[j], g->players[i]->num_cards[j]) < 21
       && g->players[i]->num_cards[j] == 2){
       printf("Player %d beat dealer!\n", i);
     printf("Player %d wins $%d\n", i, (g->players[i]->bet) 
       / g->players[i]->num_hands);
     g->players[i]->money += (2 * g->players[i]->bet) / g->players[i]->num_hands;
   }
   else if (sum_cards(g->players[i]->hands[j], g->players[i]->num_cards[j]) == 21
     && g->players[i]->num_cards[j] == 2){
     printf("Player %d: BLACKJACK!\n", i);
   printf("Player %d wins $%d\n", i, ((int)(1.5 * (double)g->players[i]->bet)) 
     / g->players[i]->num_hands);
   g->players[i]->money += ((int)(2.5 * (double)g->players[i]->bet)) 
   / g->players[i]->num_hands;
 }
 else{
   printf("Player %d busted!\n", i);
   printf("Player %d loses $%d\n", i, (g->players[i]->bet) / g->players[i]->num_hands);
 }
}
g->players[i]->bet = 0;
printf("\n");
}
else
  for (i = 1; i < g->num_players; i++){
    if (g->players[i]->active == 0)
     continue;
   int j;
   for (j = 0; j < g->players[i]->num_hands; j++){
     if (sum_cards(g->players[i]->hands[j], g->players[i]->num_cards[j]) == 21
       && g->players[i]->num_cards[j] == 2){
       printf("Player %d: BLACKJACK!\n", i);
     printf("Player %d wins $%d\n", i, ((int)(1.5 * (double)g->players[i]->bet)) 
       / g->players[i]->num_hands);
     g->players[i]->money += ((int)(2.5 * (double)g->players[i]->bet)) 
     / g->players[i]->num_hands;
   }

   else if (sum_cards(g->players[i]->hands[j], g->players[i]->num_cards[j]) > 21){
     printf("Player %d busted!\n", i);
     printf("Player %d loses $%d\n", i, (g->players[i]->bet) 
       / g->players[i]->num_hands);
   }

   else if (sum_cards(g->players[0]->hands[0], g->players[0]->num_cards[0]) <
     sum_cards(g->players[i]->hands[j], g->players[i]->num_cards[j])){
     printf("Player %d beat dealer!\n", i);
   printf("Player %d wins $%d\n", i,  g->players[i]->bet / g->players[i]->num_hands);
   g->players[i]->money += (2 * g->players[i]->bet)  / g->players[i]->num_hands;
 }
 else if (sum_cards(g->players[0]->hands[0], g->players[0]->num_cards[0]) >
   sum_cards(g->players[i]->hands[j], g->players[i]->num_cards[j])){
   printf("Player %d lost to dealer!\n", i);
 printf("Player %d loses $%d\n", i, (g->players[i]->bet) / g->players[i]->num_hands);
}
else {
 g->players[i]->money += g->players[i]->bet;
 printf("Player %d pushed!", i);
}
}
g->players[i]->bet = 0;
printf("\n");
}
}

void clear_hands(game g){
  int i;
  int j;
  int k;
  for (i = 0; i < g->num_players; i++){
    for (j = 0; j < g->players[i]->num_hands; j++){
      for (k = 0; k < g->players[i]->num_cards[j]; k++)
       g->players[i]->hands[j][k] = NULL;
     g->players[i]->num_cards[j] = 0;
     g->players[i]->num_hands = 1;
   }
 }
}


char* split(game g, player p, int which){
  char* charbuffer = (char*)malloc(32);
  int i;
  if(p->hands[which][0]->value != p->hands[which][1]->value){
    printf("Can't split these cards!\n");
    for (i = 0; i < 32; i++)
      charbuffer[i] = '\0';
    printf("What would you like to do?\n");
    printf("Please enter (H)it, (S)tay, or (Sp)lit: ");
    fgets(charbuffer, sizeof charbuffer, stdin);
    for(i = 0; charbuffer[i]; i++)
      charbuffer[i] = tolower(charbuffer[i]);
    return charbuffer;
  }
  else {
    if (wager(p,p->bet,g)){
      p->hands[p->num_hands][0] = p->hands[which][1];
      p->num_cards[p->num_hands]++;
      p->hands[which][1] = NULL;
      p->num_cards[which]--;
      p->num_hands++;
      deal_player(g,p,which);
      deal_player(g,p,p->num_hands-1);
      int k;
      for (k = 0; k < p->num_hands; k++){
        if (sum_cards(p->hands[k],p->num_cards[k]) == 21
         && p->num_cards[k] == 2){
          printf("Hand %d: BLACKJACK!", k+1);
        continue;
      }

      while (1){
       for (i = 0; i < 32; i++)
         charbuffer[i] = '\0';
       printf("\033[2J\033[1;1H");
         print_hand(p);
         printf("Hand %d: What would you like to do?\n",k+1);
         printf("Please enter (H)it, (S)tay, or (Sp)lit: ");
         fgets(charbuffer, sizeof charbuffer, stdin);
         for(i = 0; charbuffer[i]; i++)
           charbuffer[i] = tolower(charbuffer[i]);

         if (!strncmp(charbuffer, "double",6) || !strncmp(charbuffer, "d", 1)){
           if (wager(p,p->bet,g)){
             printf("\033[2J\033[1;1H");
               printf("Player %d: Double down!\n", p->player_number);
               deal_player(g,p,k);
               print_hand(p);
               return charbuffer;
             }
             else {
               printf("Cannot double-down\n");
               for (i = 0; i < 32; i++)
                charbuffer[i] = '\0';
              printf("Hand %d: What would you like to do?\n", k+1);
              printf("Please enter (H)it, (S)tay, or (Sp)lit: ");
              fgets(charbuffer, sizeof charbuffer, stdin);
              for(i = 0; charbuffer[i]; i++)
                charbuffer[i] = tolower(charbuffer[i]);
            }
          }
          else if (!strncmp(charbuffer, "split",5) || !strncmp(charbuffer, "sp", 2)){
           split(g,p,k);
         }
         else if (!strncmp(charbuffer, "stay",4) || !strncmp(charbuffer, "s",1)){
           break;
         }
         else if (!strncmp(charbuffer, "hit",3) || !strncmp(charbuffer, "h",1)){
           if (deal_player(g,p,k) == 0){
             print_hand(p);
             printf("\nPlayer %d busted!\n", p->player_number);
             return charbuffer;
           }
           printf("\033[2J\033[1;1H");
             for (i = 0; i < 32; i++)
               charbuffer[i] = '\0';
             printf("Dealer's top card: \n");
             print_card(g->players[0]->hands[0][1]);
             print_hand(p);
             printf("Hand %d: What would you like to do?\n",k+1);
             printf("Please enter (H)it, (S)tay, or (Sp)lit: ");
             fgets(charbuffer, sizeof charbuffer, stdin);
             for(i = 0; charbuffer[i]; i++)
               charbuffer[i] = tolower(charbuffer[i]);
           }
           else {
             for (i = 0; i < 32; i++)
               charbuffer[i] = '\0';
             printf("Invalid input!\n");
             printf("Please enter (H)it, (S)tay, (Sp)lit, or (D)ouble: ");
             fgets(charbuffer, sizeof charbuffer, stdin);
             for(i = 0; charbuffer[i]; i++)
               charbuffer[i] = tolower(charbuffer[i]);
           }
         }
       }
     }

     else{
      printf("Not enough money! Can't split!\n");
      for (i = 0; i < 32; i++)
       charbuffer[i] = '\0';
     printf("What would you like to do?\n");
     printf("Please enter (H)it, (S)tay, (Sp)lit, or (D)ouble: ");
     fgets(charbuffer, sizeof charbuffer, stdin);
     for(i = 0; charbuffer[i]; i++)
       charbuffer[i] = tolower(charbuffer[i]);
     return charbuffer;
   }
 }
 return charbuffer;
}


int move(game g, player p){

  printf("\033[2J\033[1;1H");
    if (p->money == 0 && p->bet == 0){
      return 3;
    }
    printf("Dealer's top card: \n");
    print_card(g->players[0]->hands[0][1]);


    player_data(p);
    printf("\n");
    char* charbuffer = (char*)malloc(32);
    int intbuffer;
    int i;

    if (sum_cards(p->hands[0],p->num_cards[0]) == 21){
      printf("Player %d: BLACKJACK!", p->player_number);
      free(charbuffer);
      return 2;
    }

    printf("Player %d what would you like to do?\n", p->player_number);
    printf("Please enter (H)it,(S)tay,(Sp)lit, or (D)ouble: ");
    fgets(charbuffer, sizeof charbuffer, stdin);
    for(i = 0; charbuffer[i]; i++)
      charbuffer[i] = tolower(charbuffer[i]);
    while (1){
      if (!strncmp(charbuffer, "double",6) || !strncmp(charbuffer, "d", 1)){
        if (wager(p,p->bet,g)){
         printf("\033[2J\033[1;1H");
           printf("Player %d: Double down!\n", p->player_number);
           deal_player(g,p,0);
           print_hand(p);
           free(charbuffer);
           return 4;
         }
         else {
           printf("Cannot double-down\n");
           for (i = 0; i < 32; i++)
             charbuffer[i] = '\0';
           printf("What would you like to do?\n");
           printf("Please enter (H)it, (S)tay, or (Sp)lit: ");
           fgets(charbuffer, sizeof charbuffer, stdin);
           for(i = 0; charbuffer[i]; i++)
             charbuffer[i] = tolower(charbuffer[i]);
         }
       }
       else if (!strncmp(charbuffer, "split",5) || !strncmp(charbuffer, "sp", 2)){
        charbuffer = split(g,p,0);
        return 5;
      }
      else if (!strncmp(charbuffer, "stay",4) || !strncmp(charbuffer, "s",1)){
        free(charbuffer);
        return 1;
      }
      else if (!strncmp(charbuffer, "hit",3) || !strncmp(charbuffer, "h",1)){
        if (deal_player(g,p,0) == 0){
         print_hand(p);
         printf("\nPlayer %d busted!\n", p->player_number);
         free(charbuffer);
         return 1;
       }
       printf("\033[2J\033[1;1H");
        for (i = 0; i < 32; i++)
         charbuffer[i] = '\0';
       printf("Dealer's top card: \n");
       print_card(g->players[0]->hands[0][1]);
       print_hand(p);
       printf("\n");
       printf("What would you like to do?\n");
       printf("Please enter (H)it, (S)tay, or (Sp)lit: ");
       fgets(charbuffer, sizeof charbuffer, stdin);
       for(i = 0; charbuffer[i]; i++)
         charbuffer[i] = tolower(charbuffer[i]);
     }
     else {
      for (i = 0; i < 32; i++)
       charbuffer[i] = '\0';
     printf("Invalid input!\n");
     printf("Please enter (H)it, (S)tay, (Sp)lit, or (D)ouble: ");
     fgets(charbuffer, sizeof charbuffer, stdin);
     for(i = 0; charbuffer[i]; i++)
       charbuffer[i] = tolower(charbuffer[i]);
   }
 }
 free(charbuffer);
 return 1;
}
