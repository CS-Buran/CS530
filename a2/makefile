#Filename: README.md
#CS 530, Spring 2019
#Assignment #2: SIC/XE Disassembler
#Brendan Le Tourneau: cssc0414 / 819897930
#Connor Rowland - Turn In Account: cssc0403 / 820436793
#Rahul Sharma: cssc0417 / 820943039

# Any time xed.o changes, compile into executable caled xed
CC = g++
EXEC = xed
$(EXEC) : xed.o
		$(CC) xed.o -o $(EXEC)
		rm -f *.o

# Any time xed.cpp or xed.h changes, compiles into object file, xed.o
xed.o : xed.cpp xed.h
		$(CC) -c xed.cpp

# Removes any previously existing .o files or xed executables files
clean :
		rm -f *.o core a.out $(EXEC) 
