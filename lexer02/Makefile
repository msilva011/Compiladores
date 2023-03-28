CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -pedantic

.PHONY: all build clean run

all: build

build: tokens

tokens: tokens.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f tokens

run: tokens
	./tokens
