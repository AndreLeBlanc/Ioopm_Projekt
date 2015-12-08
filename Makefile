CC=gcc
FLAGS_PROD=-Wall -std=c11
FLAGS_DEBUG=$(FLAGS) -ggdb
FLAGS_CUNIT=$(FLAGS_DEBUG) -lcunit

DIR_RESOURCES=./resources/

all:
	@echo "Not implemented yet."

#compile object files
%.o: %.c
	$(CC) $(FLAGS_PROD) -o $@ -c $^

# compile object files with debugging information
%.debug.o: %.c
	$(CC) $(FLAGS_DEBUG) -o $@ -c $^

#generate documentation with doxygen
doc: $(DIR_RESOURCES)gc.doxy
	doxygen $^
.PHONY: doc

#check if there are memory leaks
valgrind: gc_test
	valgrind --leak-check=full ./gc_test.out

#run tests
test: gc_test
	./gc_test.out
.PHONY: test

gc_test: gc_test.c gc.o
	$(CC) $(FLAGS_CUNIT) -o $@.out $^

clean:
	rm -rf *.out
	rm -rf *.o
	rm -rf ./doc/*
