.PHONY = clean all

#compiler
CC = gcc
APP_NAME = mrtsch.out
LINKERFLAG = -lm -lpthread

SRCS := lib/simulation.c lib/report.c main.c
OBJ = lib/simulation.o lib/report.o main.o



all : compile

compile : $(OBJ)
	$(CC) $(OBJ) -o $(APP_NAME) $(LINKERFLAG)

# lib/simulation.o : include/simulation.h

clean :
	rm -v lib/*.o
	rm -v *.o
	rm $(APP_NAME)


