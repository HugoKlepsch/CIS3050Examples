
CC	= gcc
CFLAGS	= -g -Wall

example : main.o myfunc.o
	$(CC) -o example main.o myfunc.o -lm

broken : main.o
	$(CC) -o broken main.o

preprocessor :
	$(CC) -E main.c -o main.E

clean : 
	- rm -f *.o example

