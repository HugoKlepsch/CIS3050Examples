#
# Build the example
#

CC	= gcc
CFLAGS	= -g -Wall

EXE	= strlen
OBJS	= strlen.o

$(EXE) : $(OBJS)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJS)

assembler :
	$(CC) -S strlen.c

preprocessor :
	$(CC) -E strlen.c -o strlen.E

clean :
	@ echo "cleaning up!"
	- rm -f $(EXE)
	- rm -f $(OBJS)
