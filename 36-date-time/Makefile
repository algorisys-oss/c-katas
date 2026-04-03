CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: datetime

datetime: exercises/datetime.c
	$(CC) $(CFLAGS) -o datetime exercises/datetime.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-datetime

sol-datetime: solutions/datetime.c
	$(CC) $(CFLAGS) -o sol-datetime solutions/datetime.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Date & Time Utilities ---"
	./sol-datetime
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f datetime
	rm -f sol-datetime

.PHONY: exercises solutions test clean
