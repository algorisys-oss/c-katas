CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: arena pool mini_malloc memory_profiler

arena: exercises/arena.c
	$(CC) $(CFLAGS) -o arena exercises/arena.c

pool: exercises/pool.c
	$(CC) $(CFLAGS) -o pool exercises/pool.c

mini_malloc: exercises/mini_malloc.c
	$(CC) $(CFLAGS) -o mini_malloc exercises/mini_malloc.c

memory_profiler: exercises/memory_profiler.c
	$(CC) $(CFLAGS) -o memory_profiler exercises/memory_profiler.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-arena sol-pool sol-mini_malloc sol-memory_profiler

sol-arena: solutions/arena.c
	$(CC) $(CFLAGS) -o sol-arena solutions/arena.c

sol-pool: solutions/pool.c
	$(CC) $(CFLAGS) -o sol-pool solutions/pool.c

sol-mini_malloc: solutions/mini_malloc.c
	$(CC) $(CFLAGS) -o sol-mini_malloc solutions/mini_malloc.c

sol-memory_profiler: solutions/memory_profiler.c
	$(CC) $(CFLAGS) -o sol-memory_profiler solutions/memory_profiler.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Arena Allocator ---"
	./sol-arena
	@echo ""
	@echo "--- Pool Allocator ---"
	./sol-pool
	@echo ""
	@echo "--- Mini Malloc ---"
	./sol-mini_malloc
	@echo ""
	@echo "--- Memory Profiler ---"
	./sol-memory_profiler
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f arena pool mini_malloc memory_profiler
	rm -f sol-arena sol-pool sol-mini_malloc sol-memory_profiler

.PHONY: exercises solutions test clean
