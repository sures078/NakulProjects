CFLAGS = -Wall -Werror -g 
CC     = gcc $(CFLAGS)
SHELL  = /bin/bash
CWD    = $(shell pwd | sed 's/.*\///g')

PROGRAMS = \
	batt_main \
	puzzlebox   \


all : $(PROGRAMS)

clean :
	rm -f $(PROGRAMS) *.o

AN=a2
zip :
	rm -f $(AN)-code.zip
	cd .. && zip "$(CWD)/$(AN)-code.zip" -r "$(CWD)"
	@echo Zip created in $(AN)-code.zip
	@if (( $$(stat -c '%s' $(AN)-code.zip) > 10*(2**20) )); then echo "WARNING: $(AN)-code.zip seems REALLY big, check there are no abnormally large test files"; du -h $(AN)-code.zip; fi

# battery problem
batt_main : batt_main.o batt_update.o batt_sim.o
	$(CC) -o $@ $^

batt_main.o : batt_main.c batt.h
	$(CC) -c $<

batt_sim.o : batt_sim.c batt.h
	$(CC) -c $<

batt_update.o : batt_update.c batt.h
	$(CC) -c $<


# debugging problem
puzzlebox.o : puzzlebox.c
	$(CC) -c $<

puzzlebox : puzzlebox.o
	$(CC) -o $@ $^


# Testing Targets
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

test: test-p1 test-p2

test-p1: test-p1a test-p1b

test-p1a: test_batt_update
	@chmod u+rx test_batt_update.sh
	./test_batt_update.sh

test-p1b : batt_main
	@chmod u+rx test_batt_main.sh
	./test_batt_main.sh

test_batt_update : test_batt_update.o batt_sim.o batt_update.o
	$(CC) -o $@ $^

test-p2 : puzzlebox
	./puzzlebox input.txt

clean-tests : clean
	rm -f test-data/*.tmp test_batt_update


