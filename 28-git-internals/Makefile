CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: sha1 objects

sha1: exercises/sha1.c
	$(CC) $(CFLAGS) -o sha1 exercises/sha1.c

objects: exercises/objects.c
	$(CC) $(CFLAGS) -o objects exercises/objects.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-sha1 sol-objects

sol-sha1: solutions/sha1.c
	$(CC) $(CFLAGS) -o sol-sha1 solutions/sha1.c

sol-objects: solutions/objects.c
	$(CC) $(CFLAGS) -o sol-objects solutions/objects.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- SHA-1 Hashing & Git Blob Format ---"
	./sol-sha1
	@echo ""
	@echo "--- Git Object Store ---"
	./sol-objects
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f sha1 objects
	rm -f sol-sha1 sol-objects

.PHONY: exercises solutions test clean
