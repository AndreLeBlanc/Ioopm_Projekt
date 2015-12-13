CC=gcc
FLAGS_PROD=-Wall -std=c11
FLAGS_DEBUG=$(FLAGS_PROD) -ggdb
FLAGS_CUNIT=$(FLAGS_DEBUG) -lcunit
FLAGS_GCOV=$(FLAGS_DEBUG) --coverage

FILES_GCOV=gc_test.c gc.c

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
	@doxygen $^
.PHONY: doc

#check if there are memory leaks
valgrind: gc_test
	@valgrind --leak-check=full ./gc_test.out
.PHONY: valgrind

#check the unit-test coverage of every source file
gcov: gcov_clean $(FILES_GCOV)
	@$(CC) $(FLAGS_GCOV) -lcunit -o gcov.out $(FILES_GCOV) #compile source files with gcov data
	@./gcov.out >> /dev/null #create profile data, silence the output
	@gcov $(FILES_GCOV)
.PHONY: gcov

# this part is executed when testing on multiple machines. change dependency to your needs (ex: os_dump, valgrind, gcov)
# DEFAULT: run_test
test: os_dump
.PHONY: test

# run tests
run_test: gc_test
	@./gc_test.out
.PHONY: run_test

os_dump:
	@echo "-s : $(shell uname -s)"; \
	echo "-m : $(shell uname -m)"; \
	echo "-o : $(shell uname -o)"; \
	echo "-r : $(shell uname -r)"; \
	echo "-p : $(shell uname -p)"; \
	echo "-v : $(shell uname -v)"; \

#compile test
gc_test: gc_test.debug.o gc.debug.o
	$(CC) -o $@.out $^ $(FLAGS_CUNIT)
.PHONY: gc_test

#remove crap files.
clean: gcov_clean
	@rm -rf *.out
	@rm -rf *.o
	@rm -rf ./doc/*
	@rm -rf ./*.dSYM
	@rm -rf .DS_Store
	@rm -rf *.zip
	@rm -rf *.result.txt
	@echo "All cleaned up!"

gcov_clean:
	@rm -rf *.gc*
	@rm -rf gcov.out
