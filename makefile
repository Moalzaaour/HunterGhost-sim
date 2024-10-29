CC = gcc
CFLAGS = -Wall -pthread

all: ghost_hunter_game

ghost_hunter_game: main.o ghost.o hunter.o house.o logger.o utils.o
	$(CC) $(CFLAGS) $^ -o $@

main.o: main.c defs.h
	$(CC) $(CFLAGS) -c main.c

ghost.o: ghost.c defs.h
	$(CC) $(CFLAGS) -c ghost.c

hunter.o: hunter.c defs.h
	$(CC) $(CFLAGS) -c hunter.c

house.o: house.c defs.h
	$(CC) $(CFLAGS) -c house.c

logger.o: logger.c defs.h
	$(CC) $(CFLAGS) -c logger.c

utils.o: utils.c defs.h
	$(CC) $(CFLAGS) -c utils.c

clean:
	rm -f *.o ghost_hunter_game

