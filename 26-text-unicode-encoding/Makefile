CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: utf8 charcount text_stats

utf8: exercises/utf8.c
	$(CC) $(CFLAGS) -o utf8 exercises/utf8.c

charcount: exercises/charcount.c
	$(CC) $(CFLAGS) -o charcount exercises/charcount.c

text_stats: exercises/text_stats.c
	$(CC) $(CFLAGS) -o text_stats exercises/text_stats.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-utf8 sol-charcount sol-text_stats

sol-utf8: solutions/utf8.c
	$(CC) $(CFLAGS) -o sol-utf8 solutions/utf8.c

sol-charcount: solutions/charcount.c
	$(CC) $(CFLAGS) -o sol-charcount solutions/charcount.c

sol-text_stats: solutions/text_stats.c
	$(CC) $(CFLAGS) -o sol-text_stats solutions/text_stats.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- UTF-8 Encoder/Decoder ---"
	./sol-utf8
	@echo ""
	@echo "--- UTF-8 Character Counter ---"
	./sol-charcount
	@echo ""
	@echo "--- Text Statistics ---"
	./sol-text_stats
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f utf8 charcount text_stats
	rm -f sol-utf8 sol-charcount sol-text_stats

.PHONY: exercises solutions test clean
