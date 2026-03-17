CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: linked_list lru_cache

linked_list: exercises/linked_list.c
	$(CC) $(CFLAGS) -o linked_list exercises/linked_list.c

lru_cache: exercises/lru_cache.c
	$(CC) $(CFLAGS) -o lru_cache exercises/lru_cache.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-linked_list sol-lru_cache

sol-linked_list: solutions/linked_list.c
	$(CC) $(CFLAGS) -o sol-linked_list solutions/linked_list.c

sol-lru_cache: solutions/lru_cache.c
	$(CC) $(CFLAGS) -o sol-lru_cache solutions/lru_cache.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Singly Linked List ---"
	./sol-linked_list
	@echo ""
	@echo "--- LRU Cache ---"
	./sol-lru_cache
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f linked_list lru_cache
	rm -f sol-linked_list sol-lru_cache

.PHONY: exercises solutions test clean
