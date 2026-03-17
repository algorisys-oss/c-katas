CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: bst heap

bst: exercises/bst.c
	$(CC) $(CFLAGS) -o bst exercises/bst.c

heap: exercises/heap.c
	$(CC) $(CFLAGS) -o heap exercises/heap.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-bst sol-heap

sol-bst: solutions/bst.c
	$(CC) $(CFLAGS) -o sol-bst solutions/bst.c

sol-heap: solutions/heap.c
	$(CC) $(CFLAGS) -o sol-heap solutions/heap.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Binary Search Tree ---"
	./sol-bst
	@echo ""
	@echo "--- Min-Heap & Heap Sort ---"
	./sol-heap
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f bst heap
	rm -f sol-bst sol-heap

.PHONY: exercises solutions test clean
