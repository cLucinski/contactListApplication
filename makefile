CC = gcc
CFLAGS = -Wall -ansi -I$(IDIR)

IDIR = ./includes/
SRCDIR = ./src/
BINDIR = ./bin/

SOURCES = $(SRCDIR)*.c

all: contactList

contactList: $(BINDIR)contactList.o $(BINDIR)functionDefns.o
	$(CC) $(BINDIR)contactList.o $(BINDIR)functionDefns.o -o $(BINDIR)contactList

$(BINDIR)contactList.o: $(SRCDIR)contactList.c
	$(CC) $(CFLAGS) -c $(SRCDIR)contactList.c -o $@

$(BINDIR)functionDefns.o: $(SRCDIR)functionDefns.c
	$(CC) $(CFLAGS) -c $(SRCDIR)functionDefns.c -o $@

run:
	./bin/contactList

clean:
	rm ./bin/*.o ./bin/contactList
