.PHONY = clean all

#compiler
CC = gcc
APP_NAME = mrtsch


SRCS := lib/simulation.c main.c
OBJ = lib/simulation.o main.o



all : compile2

compile : $(OBJ)
	$(CC) $(OBJ) -o $(APP_NAME)

# lib/simulation.o : include/simulation.h

compile2: $(OBJ)
	gcc -o GUI GUI.c -lm `pkg-config --cflags --libs gtk+-3.0` -export-dynamic

clean :
	rm -v lib/*.o
	rm -v *.o
	rm $(APP_NAME)


