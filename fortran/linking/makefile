
FC		= gfortran
CFLAGS	=	-g
FFLAGS	=	-g


fortexample : testC.o testF.o
	$(FC) -o fortexample testC.o testF.o

clean :
	- rm -f testC.o testF.o
	- rm -f fortexample

