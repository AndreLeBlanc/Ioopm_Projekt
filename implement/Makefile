CC=gcc
FLAGS=-Wall -std=c11 -ggdb

all: main

main: main.o util.o list.o tree.o extern.o heap.o
	$(CC) $(FLAGS) util.o list.o tree.o extern.o heap.o  main.o -o main

test: test.o util.o list.o tree.o
	$(CC) $(FLAGS) util.o list.o tree.o test.o -o test

tests: test.o util.o
	$(CC) $(FLAGS) util.o test.o -o test

test_list: test_list.o util.o list.o
	$(CC) $(FLAGS) util.o list.o test_list.o -o test_list

test_list.o: test_list.c
	$(CC) $(FLAGS) -c test_list.c -o test_list.o

test.o: test.c
	$(CC) $(FLAGS) -c test.c -o test.o

extern.o: extern.c 
	$(CC) $(FLAGS) -c extern.c -o extern.o

main.o: main.c main.h
	$(CC) $(FLAGS) -c main.c -o main.o

tree.o: tree.c tree.h
	$(CC) $(FLAGS) -c tree.c -o tree.o

list.o: list.c list.h
	$(CC) $(FLAGS) -c list.c -o list.o

util.o: util.c util.h
	$(CC) $(FLAGS) -c util.c -o util.o

heap.o: heap.c heap.h
	$(CC) $(FLAGS) -c heap.c -o heap.o

run: main
	./main

runt: main
	./main < test.txt

debug:
	gdb ./main

clean:
	rm -f main
	rm -f test
	rm -f test_list
	rm -f *.o

clean_autosave:
	find ./ -name "*~" | xargs rm -f

# Flymake mode (Live syntax and error check)
# Insert the following lines in your .emacs file:
#	(require 'flymake)
#	(add-hook 'find-file-hook 'flymake-find-file-hook)
# To put in practical use: M-x flymake-mode RET
check-syntax:
	gcc -o nul -S ${CHK_SOURCES}
