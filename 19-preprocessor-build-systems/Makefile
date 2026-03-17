CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: macros

macros: exercises/macros.c
	$(CC) $(CFLAGS) -o macros exercises/macros.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-macros

sol-macros: solutions/macros.c
	$(CC) $(CFLAGS) -o sol-macros solutions/macros.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Utility Macros ---"
	./sol-macros
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f macros
	rm -f sol-macros

.PHONY: exercises solutions test clean
