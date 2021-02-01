CFLAGS = -Wall -g -Og
CC     = gcc $(CFLAGS)
SHELL  = /bin/bash
CWD    = $(shell pwd | sed 's/.*\///g')

PROGRAMS = \
	mult_benchmark \
	search_benchmark \


all : $(PROGRAMS)

clean :
	rm -f $(PROGRAMS) *.o vgcore.* 

AN=a4
zip :
	rm -f $(AN)-code.zip
	cd .. && zip "$(CWD)/$(AN)-code.zip" -r "$(CWD)"
	@echo Zip created in $(AN)-code.zip
	@if (( $$(stat -c '%s' $(AN)-code.zip) > 10*(2**20) )); then echo "WARNING: $(AN)-code.zip seems REALLY big, check there are no abnormally large test files"; du -h $(AN)-code.zip; fi

# Matrix-vector optimization problem
matvec_util.o : matvec_util.c matvec.h
	$(CC) -c $<

baseline_matvec_mult.o : baseline_matvec_mult.c matvec.h
	$(CC) -c $<

optimized_matvec_mult.o : optimized_matvec_mult.c matvec.h
	$(CC) -c $<

mult_benchmark : mult_benchmark.o baseline_matvec_mult.o optimized_matvec_mult.o matvec_util.o
	$(CC) -o $@ $^

mult_benchmark.o : mult_benchmark.c matvec.h
	$(CC) -c $<

# Search Algorithm Problem
search_benchmark : search_benchmark.o search_funcs.o
	$(CC) -o $@ $^

search_benchmark.o : search_benchmark.c search.h
	$(CC) -c $<

search_funcs.o : search_funcs.c search.h
	$(CC) -c $<

# Testing Targets
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

# run with valgrind to check for memory problems
p1-valgrind: 
	$(VALGRIND) ./mult_benchmark test

p2-valgrind:
	$(VALGRIND) ./search_benchmark 3 8 1 ablt
