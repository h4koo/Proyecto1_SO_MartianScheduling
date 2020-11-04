.PHONY = clean all

#compiler
CC = gcc
APP_NAME = mrtsch.out
LINKERFLAG = -lm -lpthread `pkg-config --cflags --libs gtk+-3.0` -export-dynamic

SRCS := main.c lib/simulation.c GUI/GUI.c lib/report.c
OBJ = main.c lib/simulation.o GUI/GUI.o 



all : compile

compile : $(SRCS)
	$(CC) -g -Wall $(SRCS) -o $(APP_NAME) $(LINKERFLAG)

compileGUI:
	gcc -g -o mrtsch.out main.c $(SRCS) $(LINKERFLAG)  
# lib/simulation.o : include/simulation.h

clean :
	# rm -v lib/*.o
	# rm -v *.o
	rm *_report.txt
	rm $(APP_NAME)
