CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# Default: build all exercises
all: hello about_me

# --- Exercise targets ---

hello: exercises/hello.c
	$(CC) $(CFLAGS) -o hello exercises/hello.c

about_me: exercises/about_me.c
	$(CC) $(CFLAGS) -o about_me exercises/about_me.c

# --- Solution targets (prefix with sol-) ---

sol-hello: solutions/hello.c
	$(CC) $(CFLAGS) -o sol-hello solutions/hello.c

sol-about_me: solutions/about_me.c
	$(CC) $(CFLAGS) -o sol-about_me solutions/about_me.c

# --- Run targets ---

run-hello: hello
	./hello

run-about_me: about_me
	./about_me

# --- Compilation exploration ---

explore: exercises/hello.c
	bash exercises/explore_compilation.sh

sol-explore: solutions/hello.c
	bash solutions/explore_compilation.sh

# --- Clean up ---

clean:
	rm -f hello about_me sol-hello sol-about_me
	rm -f hello.i hello.s hello.o

.PHONY: all clean explore sol-explore run-hello run-about_me
