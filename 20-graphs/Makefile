CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: graph

graph: exercises/graph.c
	$(CC) $(CFLAGS) -o graph exercises/graph.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-graph

sol-graph: solutions/graph.c
	$(CC) $(CFLAGS) -o sol-graph solutions/graph.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Graphs ---"
	./sol-graph
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f graph
	rm -f sol-graph

.PHONY: exercises solutions test clean
