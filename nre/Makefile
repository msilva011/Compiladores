CC=gcc
CFLAGS=-Wall

all: build run

build: main

main: main.c
	$(CC) $(CFLAGS) main.c -o main

run:
	./main

clean:
	rm -f main