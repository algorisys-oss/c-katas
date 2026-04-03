CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: linked_list lru_cache list_intersection

linked_list: exercises/linked_list.c
	$(CC) $(CFLAGS) -o linked_list exercises/linked_list.c

lru_cache: exercises/lru_cache.c
	$(CC) $(CFLAGS) -o lru_cache exercises/lru_cache.c

list_intersection: exercises/list_intersection.c
	$(CC) $(CFLAGS) -o list_intersection exercises/list_intersection.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-linked_list sol-lru_cache sol-list_intersection

sol-linked_list: solutions/linked_list.c
	$(CC) $(CFLAGS) -o sol-linked_list solutions/linked_list.c

sol-lru_cache: solutions/lru_cache.c
	$(CC) $(CFLAGS) -o sol-lru_cache solutions/lru_cache.c

sol-list_intersection: solutions/list_intersection.c
	$(CC) $(CFLAGS) -o sol-list_intersection solutions/list_intersection.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Singly Linked List ---"
	./sol-linked_list
	@echo ""
	@echo "--- LRU Cache ---"
	./sol-lru_cache
	@echo ""
	@echo "--- List Intersection ---"
	./sol-list_intersection
	@echo ""
	@echo "All solution tests complete."

# --- Debug exercise targets ---

debug: exercises/debug_list.c
	$(CC) $(CFLAGS) exercises/debug_list.c -o exercises/debug_list

debug-solution: solutions/debug_list.c
	$(CC) $(CFLAGS) solutions/debug_list.c -o solutions/debug_list

# --- Clean up ---

clean:
	rm -f linked_list lru_cache list_intersection
	rm -f sol-linked_list sol-lru_cache sol-list_intersection
	rm -f exercises/debug_list solutions/debug_list

.PHONY: exercises solutions test clean debug debug-solution
