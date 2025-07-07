CC = gcc
CFLAGS = -Wall -g

all: main

main: main.o
	$(CC) $(CFLAGS) -o $@ main.o

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f main.o main