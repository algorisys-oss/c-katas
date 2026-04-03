CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: factorial fibonacci power hanoi recursive_strings memoization \
	backtracking advanced_dp

factorial: exercises/factorial.c
	$(CC) $(CFLAGS) -o factorial exercises/factorial.c

fibonacci: exercises/fibonacci.c
	$(CC) $(CFLAGS) -o fibonacci exercises/fibonacci.c

power: exercises/power.c
	$(CC) $(CFLAGS) -o power exercises/power.c

hanoi: exercises/hanoi.c
	$(CC) $(CFLAGS) -o hanoi exercises/hanoi.c

recursive_strings: exercises/recursive_strings.c
	$(CC) $(CFLAGS) -o recursive_strings exercises/recursive_strings.c

memoization: exercises/memoization.c
	$(CC) $(CFLAGS) -o memoization exercises/memoization.c

backtracking: exercises/backtracking.c
	$(CC) $(CFLAGS) -o backtracking exercises/backtracking.c

advanced_dp: exercises/advanced_dp.c
	$(CC) $(CFLAGS) -o advanced_dp exercises/advanced_dp.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-factorial sol-fibonacci sol-power sol-hanoi sol-recursive_strings sol-memoization \
	sol-backtracking sol-advanced_dp

sol-factorial: solutions/factorial.c
	$(CC) $(CFLAGS) -o sol-factorial solutions/factorial.c

sol-fibonacci: solutions/fibonacci.c
	$(CC) $(CFLAGS) -o sol-fibonacci solutions/fibonacci.c

sol-power: solutions/power.c
	$(CC) $(CFLAGS) -o sol-power solutions/power.c

sol-hanoi: solutions/hanoi.c
	$(CC) $(CFLAGS) -o sol-hanoi solutions/hanoi.c

sol-recursive_strings: solutions/recursive_strings.c
	$(CC) $(CFLAGS) -o sol-recursive_strings solutions/recursive_strings.c

sol-memoization: solutions/memoization.c
	$(CC) $(CFLAGS) -o sol-memoization solutions/memoization.c

sol-backtracking: solutions/backtracking.c
	$(CC) $(CFLAGS) -o sol-backtracking solutions/backtracking.c

sol-advanced_dp: solutions/advanced_dp.c
	$(CC) $(CFLAGS) -o sol-advanced_dp solutions/advanced_dp.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Factorial ---"
	./sol-factorial
	@echo ""
	@echo "--- Fibonacci ---"
	./sol-fibonacci
	@echo ""
	@echo "--- Power ---"
	./sol-power
	@echo ""
	@echo "--- Tower of Hanoi ---"
	./sol-hanoi
	@echo ""
	@echo "--- Recursive Strings & Arrays ---"
	./sol-recursive_strings
	@echo ""
	@echo "--- Memoization & Dynamic Programming ---"
	./sol-memoization
	@echo ""
	@echo "--- Backtracking ---"
	./sol-backtracking
	@echo ""
	@echo "--- Advanced Dynamic Programming ---"
	./sol-advanced_dp
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f factorial fibonacci power hanoi recursive_strings memoization backtracking advanced_dp
	rm -f sol-factorial sol-fibonacci sol-power sol-hanoi sol-recursive_strings sol-memoization sol-backtracking sol-advanced_dp

.PHONY: exercises solutions test clean
