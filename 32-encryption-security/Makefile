CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: ciphers rsa_mini

ciphers: exercises/ciphers.c
	$(CC) $(CFLAGS) -o ciphers exercises/ciphers.c

rsa_mini: exercises/rsa_mini.c
	$(CC) $(CFLAGS) -o rsa_mini exercises/rsa_mini.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-ciphers sol-rsa_mini

sol-ciphers: solutions/ciphers.c
	$(CC) $(CFLAGS) -o sol-ciphers solutions/ciphers.c

sol-rsa_mini: solutions/rsa_mini.c
	$(CC) $(CFLAGS) -o sol-rsa_mini solutions/rsa_mini.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Classical Ciphers ---"
	./sol-ciphers
	@echo ""
	@echo "--- Mini RSA ---"
	./sol-rsa_mini
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f ciphers rsa_mini
	rm -f sol-ciphers sol-rsa_mini

.PHONY: exercises solutions test clean
