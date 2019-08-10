#Connor Rowland, CSSC0403, 820436793
#CS 530, Spring 2019
#Lab #3, Simple Parser
#FILENAME:makefile

EXEC = exp
CC = g++

$(EXEC).tab.c $(EXEC).tab.h: $(EXEC).y
	bison -d $(EXEC).y
lex.yy.c: $(EXEC).l $(EXEC).tab.h
	flex $(EXEC).l

$(EXEC): lex.yy.c $(EXEC).tab.c $(EXEC).tab.h
	$(CC) $(EXEC).tab.c lex.yy.c -o $(EXEC) 
