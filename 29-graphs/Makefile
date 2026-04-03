CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: graph maze topo_sort grid_problems

graph: exercises/graph.c
	$(CC) $(CFLAGS) -o graph exercises/graph.c

maze: exercises/maze.c
	$(CC) $(CFLAGS) -o maze exercises/maze.c

topo_sort: exercises/topo_sort.c
	$(CC) $(CFLAGS) -o topo_sort exercises/topo_sort.c

grid_problems: exercises/grid_problems.c
	$(CC) $(CFLAGS) -o grid_problems exercises/grid_problems.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-graph sol-maze sol-topo_sort sol-grid_problems

sol-graph: solutions/graph.c
	$(CC) $(CFLAGS) -o sol-graph solutions/graph.c

sol-maze: solutions/maze.c
	$(CC) $(CFLAGS) -o sol-maze solutions/maze.c

sol-topo_sort: solutions/topo_sort.c
	$(CC) $(CFLAGS) -o sol-topo_sort solutions/topo_sort.c

sol-grid_problems: solutions/grid_problems.c
	$(CC) $(CFLAGS) -o sol-grid_problems solutions/grid_problems.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Graphs ---"
	./sol-graph
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
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f graph maze topo_sort grid_problems
	rm -f sol-graph sol-maze sol-topo_sort sol-grid_problems

.PHONY: exercises solutions test clean
