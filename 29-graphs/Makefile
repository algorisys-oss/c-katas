CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: graph graph_advanced maze topo_sort grid_problems union_find

graph: exercises/graph.c
	$(CC) $(CFLAGS) -o graph exercises/graph.c

graph_advanced: exercises/graph_advanced.c
	$(CC) $(CFLAGS) -o graph_advanced exercises/graph_advanced.c

maze: exercises/maze.c
	$(CC) $(CFLAGS) -o maze exercises/maze.c

topo_sort: exercises/topo_sort.c
	$(CC) $(CFLAGS) -o topo_sort exercises/topo_sort.c

grid_problems: exercises/grid_problems.c
	$(CC) $(CFLAGS) -o grid_problems exercises/grid_problems.c

union_find: exercises/union_find.c
	$(CC) $(CFLAGS) -o union_find exercises/union_find.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-graph sol-graph_advanced sol-maze sol-topo_sort sol-grid_problems sol-union_find

sol-graph: solutions/graph.c
	$(CC) $(CFLAGS) -o sol-graph solutions/graph.c

sol-graph_advanced: solutions/graph_advanced.c
	$(CC) $(CFLAGS) -o sol-graph_advanced solutions/graph_advanced.c

sol-maze: solutions/maze.c
	$(CC) $(CFLAGS) -o sol-maze solutions/maze.c

sol-topo_sort: solutions/topo_sort.c
	$(CC) $(CFLAGS) -o sol-topo_sort solutions/topo_sort.c

sol-grid_problems: solutions/grid_problems.c
	$(CC) $(CFLAGS) -o sol-grid_problems solutions/grid_problems.c

sol-union_find: solutions/union_find.c
	$(CC) $(CFLAGS) -o sol-union_find solutions/union_find.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Graphs ---"
	./sol-graph
	@echo ""
	@echo "--- Advanced Graphs ---"
	./sol-graph_advanced
	@echo ""
	@echo "--- Maze Solver ---"
	./sol-maze
	@echo ""
	@echo "--- Topological Sort ---"
	./sol-topo_sort
	@echo ""
	@echo "--- Grid Problems ---"
	./sol-grid_problems
	@echo ""
	@echo "--- Union-Find ---"
	./sol-union_find
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f graph graph_advanced maze topo_sort grid_problems union_find
	rm -f sol-graph sol-graph_advanced sol-maze sol-topo_sort sol-grid_problems sol-union_find

.PHONY: exercises solutions test clean
