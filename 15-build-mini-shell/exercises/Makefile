CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra -pedantic -g

all: tokenizer mini_shell

tokenizer: tokenizer.c
	$(CC) $(CFLAGS) -o $@ $<

mini_shell: mini_shell.c
	$(CC) $(CFLAGS) -o $@ $<

test-tokenizer: tokenizer
	./tokenizer

test-shell: mini_shell
	./mini_shell --test

test: test-tokenizer test-shell

clean:
	rm -f tokenizer mini_shell
	rm -f /tmp/mini_shell_test_*.txt

.PHONY: all clean test test-tokenizer test-shell
