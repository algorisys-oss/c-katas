CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -pthread

# --- Exercise targets ---

exercises: producer_consumer parallel_sum

producer_consumer: exercises/producer_consumer.c
	$(CC) $(CFLAGS) -o producer_consumer exercises/producer_consumer.c

parallel_sum: exercises/parallel_sum.c
	$(CC) $(CFLAGS) -o parallel_sum exercises/parallel_sum.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-producer_consumer sol-parallel_sum

sol-producer_consumer: solutions/producer_consumer.c
	$(CC) $(CFLAGS) -o sol-producer_consumer solutions/producer_consumer.c

sol-parallel_sum: solutions/parallel_sum.c
	$(CC) $(CFLAGS) -o sol-parallel_sum solutions/parallel_sum.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Producer-Consumer ---"
	./sol-producer_consumer
	@echo ""
	@echo "--- Parallel Sum ---"
	./sol-parallel_sum
	@echo ""
	@echo "All solution tests complete."

# --- Debug exercise targets ---

debug: exercises/debug_concurrency.c
	$(CC) $(CFLAGS) exercises/debug_concurrency.c -o exercises/debug_concurrency -lpthread

debug-solution: solutions/debug_concurrency.c
	$(CC) $(CFLAGS) solutions/debug_concurrency.c -o solutions/debug_concurrency -lpthread

# --- Clean up ---

clean:
	rm -f producer_consumer parallel_sum
	rm -f sol-producer_consumer sol-parallel_sum
	rm -f exercises/debug_concurrency solutions/debug_concurrency

.PHONY: exercises solutions test clean debug debug-solution
