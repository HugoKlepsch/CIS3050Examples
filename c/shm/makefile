
CFLAGS = -g -Wall

all : shmWriter shmViewer

shmWriter : shm-writer.o
	$(CC) -o shmWriter shm-writer.o

shmViewer : shm-viewer.o
	$(CC) -o shmViewer shm-viewer.o

data1.dat data1 :
	dd of=data1.dat if=/dev/zero bs=1 count=1

data512.dat data512 :
	dd of=data512.dat if=/dev/zero bs=512 count=1

data1k.dat data1k :
	dd of=data1k.dat if=/dev/zero bs=1k count=1

clean:
	- rm -f shmWriter shm-writer.o
	- rm -f shmViewer shm-viewer.o
	- rm -f tags

tags ctags: *.c
	- ctags *.c

