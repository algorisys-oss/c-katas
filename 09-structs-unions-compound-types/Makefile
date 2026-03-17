CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: structs tagged_union point

structs: exercises/structs.c
	$(CC) $(CFLAGS) -o structs exercises/structs.c

tagged_union: exercises/tagged_union.c
	$(CC) $(CFLAGS) -o tagged_union exercises/tagged_union.c

point: exercises/point.c
	$(CC) $(CFLAGS) -o point exercises/point.c -lm

# --- Solution targets (prefix with sol-) ---

solutions: sol-structs sol-tagged_union sol-point

sol-structs: solutions/structs.c
	$(CC) $(CFLAGS) -o sol-structs solutions/structs.c

sol-tagged_union: solutions/tagged_union.c
	$(CC) $(CFLAGS) -o sol-tagged_union solutions/tagged_union.c

sol-point: solutions/point.c
	$(CC) $(CFLAGS) -o sol-point solutions/point.c -lm

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Student Records ---"
	./sol-structs
	@echo ""
	@echo "--- Tagged Union ---"
	./sol-tagged_union
	@echo ""
	@echo "--- 2D Point/Vector ---"
	./sol-point
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f structs tagged_union point
	rm -f sol-structs sol-tagged_union sol-point

.PHONY: exercises solutions test clean
