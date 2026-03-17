CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: hash_table word_freq

hash_table: exercises/hash_table.c
	$(CC) $(CFLAGS) -o hash_table exercises/hash_table.c

word_freq: exercises/word_freq.c
	$(CC) $(CFLAGS) -o word_freq exercises/word_freq.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-hash_table sol-word_freq

sol-hash_table: solutions/hash_table.c
	$(CC) $(CFLAGS) -o sol-hash_table solutions/hash_table.c

sol-word_freq: solutions/word_freq.c
	$(CC) $(CFLAGS) -o sol-word_freq solutions/word_freq.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Hash Table ---"
	./sol-hash_table
	@echo ""
	@echo "--- Word Frequency Counter ---"
	./sol-word_freq
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f hash_table word_freq
	rm -f sol-hash_table sol-word_freq

.PHONY: exercises solutions test clean
