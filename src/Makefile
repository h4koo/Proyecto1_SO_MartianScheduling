.PHONY = clean all

#compiler
CC = gcc
APP_NAME = mrtsch.out
LINKERFLAG = -lm -lpthread

SRCS := lib/simulation.c GUI/GUI.c
OBJ = lib/simulation.o GUI/GUI.o



all : compile

compile : $(OBJ)
	$(CC) $(OBJ) -o $(APP_NAME) $(LINKERFLAG)

compileGUI:
	gcc -o mrtsch.out main.c $(SRCS) -lm `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
# lib/simulation.o : include/simulation.h

clean :
	rm -v lib/*.o
	rm -v *.o
	rm $(APP_NAME)
