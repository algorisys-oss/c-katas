CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: mini_shell

mini_shell: exercises/mini_shell.c
	$(CC) $(CFLAGS) -o mini_shell exercises/mini_shell.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-mini_shell

sol-mini_shell: solutions/mini_shell.c
	$(CC) $(CFLAGS) -o sol-mini_shell solutions/mini_shell.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Mini Shell (Parsing) ---"
	./sol-mini_shell
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f mini_shell
	rm -f sol-mini_shell

.PHONY: exercises solutions test clean
