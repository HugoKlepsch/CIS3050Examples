
CFLAGS=`python2.7-config --cflags` -I/usr/include/python2.7
LDFLAGS=`python2.7-config --ldflags`

call :  callMain.o
	$(CC) callMain.o -o call $(LDFLAGS)

call2 :  callMainNew.o
	$(CC) callMainNew.o -o call2 $(LDFLAGS)

clean:
	- rm -f callMain.o call
	- rm -f *.pyc

