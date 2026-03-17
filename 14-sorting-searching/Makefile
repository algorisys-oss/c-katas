CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: sorting binary_search

sorting: exercises/sorting.c
	$(CC) $(CFLAGS) -o sorting exercises/sorting.c

binary_search: exercises/binary_search.c
	$(CC) $(CFLAGS) -o binary_search exercises/binary_search.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-sorting sol-binary_search

sol-sorting: solutions/sorting.c
	$(CC) $(CFLAGS) -o sol-sorting solutions/sorting.c

sol-binary_search: solutions/binary_search.c
	$(CC) $(CFLAGS) -o sol-binary_search solutions/binary_search.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Sorting Algorithms ---"
	./sol-sorting
	@echo ""
	@echo "--- Binary Search ---"
	./sol-binary_search
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f sorting binary_search
	rm -f sol-sorting sol-binary_search

.PHONY: exercises solutions test clean
