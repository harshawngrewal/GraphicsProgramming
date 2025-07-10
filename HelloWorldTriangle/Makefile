CC = gcc
CFLAGS = -Wall -g -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl
all: main

main: glad.o main.o
	$(CC) $(CFLAGS) -o $@ main.o glad.o

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $<

glad.o: glad.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f main.o main