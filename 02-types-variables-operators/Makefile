CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# Default: build all exercises
all: exercises

# --- Exercise targets ---

exercises: type_explorer bit_operations float_mysteries

type_explorer: exercises/type_explorer.c
	$(CC) $(CFLAGS) -o type_explorer exercises/type_explorer.c

bit_operations: exercises/bit_operations.c
	$(CC) $(CFLAGS) -o bit_operations exercises/bit_operations.c

float_mysteries: exercises/float_mysteries.c
	$(CC) $(CFLAGS) -o float_mysteries exercises/float_mysteries.c -lm

# --- Solution targets ---

solutions: sol-type_explorer sol-bit_operations sol-float_mysteries

sol-type_explorer: solutions/type_explorer.c
	$(CC) $(CFLAGS) -o sol-type_explorer solutions/type_explorer.c

sol-bit_operations: solutions/bit_operations.c
	$(CC) $(CFLAGS) -o sol-bit_operations solutions/bit_operations.c

sol-float_mysteries: solutions/float_mysteries.c
	$(CC) $(CFLAGS) -o sol-float_mysteries solutions/float_mysteries.c -lm

# --- Run targets ---

run-type_explorer: type_explorer
	./type_explorer

run-bit_operations: bit_operations
	./bit_operations

run-float_mysteries: float_mysteries
	./float_mysteries

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "=== Running sol-type_explorer ==="
	@./sol-type_explorer
	@echo ""
	@echo "=== Running sol-bit_operations ==="
	@./sol-bit_operations
	@echo ""
	@echo "=== Running sol-float_mysteries ==="
	@./sol-float_mysteries

# --- Clean up ---

clean:
	rm -f type_explorer bit_operations float_mysteries
	rm -f sol-type_explorer sol-bit_operations sol-float_mysteries

.PHONY: all exercises solutions test clean \
        run-type_explorer run-bit_operations run-float_mysteries
