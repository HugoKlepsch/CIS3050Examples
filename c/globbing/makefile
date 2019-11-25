#
# Build our two globbing examples
#

CC = gcc
CFLAGS = -g

all : globsource globletters

globsource : globsource.o
	$(CC) $(CFLAGS) -o globsource globsource.o

globletters : globletters.o
	$(CC) $(CFLAGS) -o globletters globletters.o

clean :
	@ echo "cleaning up!"
	- rm -f globsource.o globsource
	- rm -f globletters.o globletters

