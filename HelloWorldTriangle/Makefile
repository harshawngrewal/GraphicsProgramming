CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm
all: main

main: glad.o main.o
	$(CC) $(CFLAGS) -o $@ main.o glad.o $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $<

glad.o: glad.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f main.o main