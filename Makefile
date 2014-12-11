#makefile for greco casino

COMPILER = gcc
FLAGS = -Wall -Wextra -Werror -Wempty-body
BJ_ARGS = blackjack.o barebones.o network.o

all: blackjack

blackjack: $(BJ_ARGS)
	$(COMPILER) $(FLAGS) -o blackjack $(BJ_ARGS)

barebones.o: barebones.c barebones.h
network.o: network.c network.h barebones.h
blackjack.o: barebones.h blackjack.c

clean: 
	rm -f *~ *.o