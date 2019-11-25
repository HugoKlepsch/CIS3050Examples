#

LIBDIR		= ./niceString
LIBSHORT	= ns
LIBNAME		= lib$(LIBSHORT).a
LIB			= $(LIBDIR)/lib/$(LIBNAME)

EXE			= hello

CXX			= g++
CXXFLAGS	= -g -I./$(LIBDIR)/include

OBJS		= main.o


$(EXE) : $(OBJS) $(LIB)
	$(CXX) -o $(EXE) $(OBJS) -L./$(LIBDIR)/lib -l$(LIBSHORT)


$(LIB) : dummy
	cd $(LIBDIR) ; make $(LIBNAME)

clean :
	- rm -f $(OBJS)
	- rm -f $(EXE)
	- rm -f *.o core.*

allclean : clean
	cd $(LIBDIR) ; make clean

ctags tags : dummy
	ctags *.c $(LIBDIR)/*.c

libdummy:
	(cd $(LIBDIR) ; make)

dummy:


