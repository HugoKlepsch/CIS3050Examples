#
# Makefile to build nice string library
#

CXX			= g++
CC			= gcc
CFLAGS		= -g -Wall -I./include
CXXFLAGS	= $(CFLAGS)

OBJS	= \
		src/growList.o \
		src/CountedMemoryReference.o \
		src/NiceString-printf.o \
		src/NiceString.o

TARGET	= libns.a


lib/$(TARGET) $(TARGET) : $(OBJS)
	ar rv lib/$(TARGET) $(OBJS)
	ranlib lib/$(TARGET)

clean :
	- rm -f $(OBJS)
	- rm -f $(TARGET)

