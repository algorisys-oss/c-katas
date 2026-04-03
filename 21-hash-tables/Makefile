CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: hash_table word_freq two_sum lru_cache

hash_table: exercises/hash_table.c
	$(CC) $(CFLAGS) -o hash_table exercises/hash_table.c

word_freq: exercises/word_freq.c
	$(CC) $(CFLAGS) -o word_freq exercises/word_freq.c

two_sum: exercises/two_sum.c
	$(CC) $(CFLAGS) -o two_sum exercises/two_sum.c

lru_cache: exercises/lru_cache.c
	$(CC) $(CFLAGS) -o lru_cache exercises/lru_cache.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-hash_table sol-word_freq sol-two_sum sol-lru_cache

sol-hash_table: solutions/hash_table.c
	$(CC) $(CFLAGS) -o sol-hash_table solutions/hash_table.c

sol-word_freq: solutions/word_freq.c
	$(CC) $(CFLAGS) -o sol-word_freq solutions/word_freq.c

sol-two_sum: solutions/two_sum.c
	$(CC) $(CFLAGS) -o sol-two_sum solutions/two_sum.c

sol-lru_cache: solutions/lru_cache.c
	$(CC) $(CFLAGS) -o sol-lru_cache solutions/lru_cache.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Hash Table ---"
	./sol-hash_table
	@echo ""
	@echo "--- Word Frequency Counter ---"
	./sol-word_freq
	@echo ""
	@echo "--- Two Sum ---"
	./sol-two_sum
	@echo ""
	@echo "--- LRU Cache ---"
	./sol-lru_cache
	@echo ""
	@echo "All solution tests complete."

# --- Debug exercise targets ---

debug: exercises/debug_hashtable.c
	$(CC) $(CFLAGS) exercises/debug_hashtable.c -o exercises/debug_hashtable

debug-solution: solutions/debug_hashtable.c
	$(CC) $(CFLAGS) solutions/debug_hashtable.c -o solutions/debug_hashtable

# --- Clean up ---

clean:
	rm -f hash_table word_freq two_sum lru_cache
	rm -f sol-hash_table sol-word_freq sol-two_sum sol-lru_cache
	rm -f exercises/debug_hashtable solutions/debug_hashtable

.PHONY: exercises solutions test clean debug debug-solution
