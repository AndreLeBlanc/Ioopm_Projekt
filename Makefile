CC=gcc
FLAGS_PROD=-Wall -std=c11
FLAGS_DEBUG=$(FLAGS) -ggdb

all:
	@echo "Not implemented yet."

%.debug.o: %.c
	$(CC) $(FLAGS_DEBUG) -o $@ -c $^

%.o: %.c
	$(CC) $(FLAGS_PROD) -o $@ -c $^

clean:
	rm -rf *.o
