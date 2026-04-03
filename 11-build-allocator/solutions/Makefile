CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g

TARGETS = arena pool stack_alloc

all: $(TARGETS)

arena: arena.c
	$(CC) $(CFLAGS) -o $@ $<

pool: pool.c
	$(CC) $(CFLAGS) -o $@ $<

stack_alloc: stack_alloc.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

.PHONY: all clean
