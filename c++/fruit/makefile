

EXE			=	fruitinfo

CXX			=	g++
CXXFLAGS	=	-g -Wall

OBJS		=	main.o fruit.o

$(EXE) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJS)

clean :
	- rm -f $(OBJS) $(EXE)

tags ctags : dummy
	- ctags *.cpp

dummy :

