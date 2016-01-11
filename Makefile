CC=gcc
FLAGS_PROD=-Wall -std=c99
FLAGS_DEBUG=$(FLAGS_PROD) -ggdb
FLAGS_CUNIT=$(FLAGS_DEBUG) -lcunit
FLAGS_GCOV=$(FLAGS_DEBUG) --coverage

FILES_TEST=traverser.debug.o linked_list.debug.o heap.debug.o gc.debug.o stack_traverser.debug.o utilities.debug.o collector.debug.o
FILES_GCOV=gc_test.c gc.c heap.c linked_list.c traverser.c stack_traverser.c utilities.c collector.c
FILES_MAIN=gc.o collector.o heap.o traverser.o utilities.o linked_list.o
FILES_H=linked_list.h stack_traverser.h heap.h

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
	@valgrind --leak-check=full ./gc_test.run
.PHONY: valgrind

#check the unit-test coverage of every source file
gcov: gcov_clean $(FILES_GCOV)
	@$(CC) $(FLAGS_GCOV) -o gcov.run $(FILES_GCOV) -lcunit #compile source files with gcov data
	@./gcov.run >> /dev/null #create profile data, silence the output
	@gcov $(FILES_GCOV)
.PHONY: gcov

#check the unit-test coverage of every source file with visual represenation created in cov/
lcov: gcov_clean gc_test $(FILES_GCOV)
	@$(CC) $(FLAGS_GCOV) -o lcov.run $(FILES_GCOV) -lcunit #compile source files with gcov data
	@./lcov.run >> /dev/null #create profile data, silence the output
	@lcov --capture --directory . --output-file coverage.info
	@genhtml coverage.info --output-directory cov
.PHONY: lcov

# this part is executed when testing on multiple machines. change dependency to your needs (ex: os_dump, valgrind, gcov)
# DEFAULT: run_test
# test: stack_c stack_test_c stack_run stack_test_run
test: stack_test
.PHONY: test

# Flymake mode (Live syntax and error check)
# Insert the following lines in your .emacs file:
#	(require 'flymake)
#	(add-hook 'find-file-hook 'flymake-find-file-hook)
# To put in practical use: M-x flymake-mode RET
check-syntax:
	gcc -o nul -S ${CHK_SOURCES}

# run tests
run_test: gc_test
	@./gc_test.run
.PHONY: run_test

os_dump:
	@echo "-s : $(shell uname -s)"; \
	echo "-m : $(shell uname -m)"; \
	echo "-o : $(shell uname -o)"; \
	echo "-r : $(shell uname -r)"; \
	echo "-p : $(shell uname -p)"; \
	echo "-v : $(shell uname -v)"; \

#compile test
gc_test: $(FILES_TEST) gc_test.c
	$(CC) -o $@.run $^ $(FLAGS_CUNIT)
.PHONY: gc_test

#compile test
linked_list.o: linked_list.c linked_list.h
	$(CC) $(FLAGS_PROD) linked_list.c -c

heap.o: heap.c heap.h
	$(CC) $(FLAGS_PROD) heap.c -o heap.o -c

stack_c: stack_traverser.o linked_list.o heap.o
	$(CC) $(FLAGS_DEBUG) -o stack_traverser stack_traverser.c

stack_run:
	@./stack_traverser
	$(CC) $(FLAGS_PROD) heap.c -c

stack_traverser.o: stack_traverser.c stack_traverser.h
	$(CC) $(FLAGS_PROD) stack_traverser.c -c

stack: stack_traverser.h heap.o linked_list.o stack_traverser.c
	$(CC) $^ $(FLAGS_DEBUG) -o stack.run

#test with gui
test_gui: $(FILES_MAIN) gui.c
	$(CC) $(FLAGS_PROD) -o $@.run $^
	@./$@.run

#remove crap files.
clean: gcov_clean
	@rm -rf *.run
	@rm -rf *.o
	@rm -rf ./doc/*
	@rm -rf ./*.dSYM
	@rm -rf .DS_Store
	@rm -rf *.zip
	@rm -rf ./resources/*.zip
	@rm -rf ./*~
	@rm -rf *.run
	@rm -rf *.out
	@rm -rf *.info
	@rm -rf ./resources/*.result.txt
	@echo "All cleaned up!"

gcov_clean:
	@rm -rf *.gc*
	@rm -rf gcov.run
