CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: btree kvstore

btree: exercises/btree.c
	$(CC) $(CFLAGS) -o btree exercises/btree.c

kvstore: exercises/kvstore.c
	$(CC) $(CFLAGS) -o kvstore exercises/kvstore.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-btree sol-kvstore

sol-btree: solutions/btree.c
	$(CC) $(CFLAGS) -o sol-btree solutions/btree.c

sol-kvstore: solutions/kvstore.c
	$(CC) $(CFLAGS) -o sol-kvstore solutions/kvstore.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- B-Tree ---"
	./sol-btree
	@echo ""
	@echo "--- Key-Value Store ---"
	./sol-kvstore
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f btree kvstore
	rm -f sol-btree sol-kvstore

.PHONY: exercises solutions test clean
