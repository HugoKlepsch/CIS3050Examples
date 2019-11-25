

EXEA		=	hello-printf
EXEB		=	hello-stream
EXEC		=	hello-aclass
EXED		=	hello

CXX			=	g++
CXXFLAGS	=	-g -Wall

OBJA		=	main-printf.o
OBJB		=	main-stream.o
OBJC		=	main-aclass.o
OBJD		=	hello.o

all : $(EXEA) $(EXEB) $(EXEC) $(EXED)

$(EXEA) : $(OBJA) 
	$(CXX) $(CXXFLAGS) -o $(EXEA) $(OBJA)

$(EXEB) : $(OBJB) 
	$(CXX) $(CXXFLAGS) -o $(EXEB) $(OBJB)

$(EXEC) : $(OBJC) 
	$(CXX) $(CXXFLAGS) -o $(EXEC) $(OBJC)

$(EXED) : $(OBJD) 
	$(CXX) $(CXXFLAGS) -o $(EXED) $(OBJD)

clean : 
	- rm -f $(OBJA) $(EXEA)
	- rm -f $(OBJB) $(EXEB)
	- rm -f $(OBJC) $(EXEC)
	- rm -f $(OBJD) $(EXED)

tags ctags : dummy
	- ctags *.cpp

dummy :

