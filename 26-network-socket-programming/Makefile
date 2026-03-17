CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: http_parser protocol

http_parser: exercises/http_parser.c
	$(CC) $(CFLAGS) -o http_parser exercises/http_parser.c

protocol: exercises/protocol.c
	$(CC) $(CFLAGS) -o protocol exercises/protocol.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-http_parser sol-protocol

sol-http_parser: solutions/http_parser.c
	$(CC) $(CFLAGS) -o sol-http_parser solutions/http_parser.c

sol-protocol: solutions/protocol.c
	$(CC) $(CFLAGS) -o sol-protocol solutions/protocol.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- HTTP Parser ---"
	./sol-http_parser
	@echo ""
	@echo "--- Protocol Parser ---"
	./sol-protocol
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f http_parser protocol
	rm -f sol-http_parser sol-protocol

.PHONY: exercises solutions test clean
