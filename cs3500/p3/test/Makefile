#
#Makefile: if you use gcc, then change g++ to gcc
OBJS = p2.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
programOne : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o p2
programOne.o : p2.h
	$(CC) $(CFLAGS) p2.cpp
clean:
	\rm *.o *~ 