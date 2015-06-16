# -O5 - niezbyt bezpieczny - przestaje prawid³owo rzucaæ wyj±tkami - Linux - gcc version pgcc-2.91.57 19980901 (egcs-1.1)

CFLAGS = -pipe -O4# -W -Wall -ggdb -DDEBUG
LFLAGS =# -lxnet
CC = g++
SHELL =	/bin/sh

OBJECTS = SString.o Exception.o SystemException.o EOFException.o FileInfo.o \
	Descriptor.o File.o Socket.o AsyncSocket.o AsyncMessageQueue.o \
	String.o Vector.o Properties.o Configuration.o \
	AsyncConnection.o ConnectionsManager.o HTTPConnection.o HTTPRequest.o \
	ConnectionWithChild.o ParentThread.o ChildThread.o \
	execParentThread.o execChildThread.o main.o
TARGET	= ../yhttpd

.SUFFIXES: .cpp

.cpp.o:
	$(CC) -c $(CFLAGS) $<

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $(TARGET)
	@rm -rf ../debug ../yhttpd.pid

depend:
	@$(CC) $(CFLAGS) -MM *.cpp >.make_depend

clean:
	@rm -f *.s *.o *.bak *~ *% #*
	@rm -f $(TARGET) a.out core error.log debug.log

include .make_depend

