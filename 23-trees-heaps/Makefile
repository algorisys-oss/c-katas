CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: bst heap prefix_sums bit_tricks segment_tree fenwick_tree

bst: exercises/bst.c
	$(CC) $(CFLAGS) -o bst exercises/bst.c

heap: exercises/heap.c
	$(CC) $(CFLAGS) -o heap exercises/heap.c

prefix_sums: exercises/prefix_sums.c
	$(CC) $(CFLAGS) -o prefix_sums exercises/prefix_sums.c

bit_tricks: exercises/bit_tricks.c
	$(CC) $(CFLAGS) -o bit_tricks exercises/bit_tricks.c

segment_tree: exercises/segment_tree.c
	$(CC) $(CFLAGS) -o segment_tree exercises/segment_tree.c

fenwick_tree: exercises/fenwick_tree.c
	$(CC) $(CFLAGS) -o fenwick_tree exercises/fenwick_tree.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-bst sol-heap sol-prefix_sums sol-bit_tricks sol-segment_tree sol-fenwick_tree

sol-bst: solutions/bst.c
	$(CC) $(CFLAGS) -o sol-bst solutions/bst.c

sol-heap: solutions/heap.c
	$(CC) $(CFLAGS) -o sol-heap solutions/heap.c

sol-prefix_sums: solutions/prefix_sums.c
	$(CC) $(CFLAGS) -o sol-prefix_sums solutions/prefix_sums.c

sol-bit_tricks: solutions/bit_tricks.c
	$(CC) $(CFLAGS) -o sol-bit_tricks solutions/bit_tricks.c

sol-segment_tree: solutions/segment_tree.c
	$(CC) $(CFLAGS) -o sol-segment_tree solutions/segment_tree.c

sol-fenwick_tree: solutions/fenwick_tree.c
	$(CC) $(CFLAGS) -o sol-fenwick_tree solutions/fenwick_tree.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Binary Search Tree ---"
	./sol-bst
	@echo ""
	@echo "--- Min-Heap & Heap Sort ---"
	./sol-heap
	@echo ""
	@echo "--- Prefix Sums & Range Queries ---"
	./sol-prefix_sums
	@echo ""
	@echo "--- Bit Manipulation Tricks ---"
	./sol-bit_tricks
	@echo ""
	@echo "--- Segment Tree ---"
	./sol-segment_tree
	@echo ""
	@echo "--- Fenwick Tree ---"
	./sol-fenwick_tree
	@echo ""
	@echo "All solution tests complete."

# --- Debug exercise targets ---

debug: exercises/debug_bst.c
	$(CC) $(CFLAGS) exercises/debug_bst.c -o exercises/debug_bst

debug-solution: solutions/debug_bst.c
	$(CC) $(CFLAGS) solutions/debug_bst.c -o solutions/debug_bst

# --- Clean up ---

clean:
	rm -f bst heap prefix_sums bit_tricks segment_tree fenwick_tree
	rm -f sol-bst sol-heap sol-prefix_sums sol-bit_tricks sol-segment_tree sol-fenwick_tree
	rm -f exercises/debug_bst solutions/debug_bst

.PHONY: exercises solutions test clean debug debug-solution
