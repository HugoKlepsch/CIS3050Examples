
CFLAGS = -g -Wall

all : mmapWriter mmapViewer

mmapWriter : mmap-writer.o
	$(CC) -o mmapWriter mmap-writer.o

mmapViewer : mmap-viewer.o
	$(CC) -o mmapViewer mmap-viewer.o

data1.dat data1 :
	dd of=data1.dat if=/dev/zero bs=1 count=1

data512.dat data512 :
	dd of=data512.dat if=/dev/zero bs=512 count=1

data1k.dat data1k :
	dd of=data1k.dat if=/dev/zero bs=1k count=1

clean:
	- rm -f mmapWriter mmap-writer.o
	- rm -f mmapViewer mmap-viewer.o
	- rm -f tags

tags ctags: *.c
	- ctags *.c

