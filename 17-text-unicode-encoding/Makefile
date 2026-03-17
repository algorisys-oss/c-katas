CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: utf8

utf8: exercises/utf8.c
	$(CC) $(CFLAGS) -o utf8 exercises/utf8.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-utf8

sol-utf8: solutions/utf8.c
	$(CC) $(CFLAGS) -o sol-utf8 solutions/utf8.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- UTF-8 Encoder/Decoder ---"
	./sol-utf8
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f utf8
	rm -f sol-utf8

.PHONY: exercises solutions test clean
