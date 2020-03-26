#Anthony Caballero
#Makefile: submit HW2 simple shell
#cpsc3500 
#1/23/18

CC = gcc
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

p1:
	$(CC) $(LFLAGS) -o p1 p1.c

clean:
	/rm*.o*~
	