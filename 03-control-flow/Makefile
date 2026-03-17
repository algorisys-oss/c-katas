CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: fizzbuzz number_guess patterns prime_checker

fizzbuzz: exercises/fizzbuzz.c
	$(CC) $(CFLAGS) -o fizzbuzz exercises/fizzbuzz.c

number_guess: exercises/number_guess.c
	$(CC) $(CFLAGS) -o number_guess exercises/number_guess.c

patterns: exercises/patterns.c
	$(CC) $(CFLAGS) -o patterns exercises/patterns.c

prime_checker: exercises/prime_checker.c
	$(CC) $(CFLAGS) -o prime_checker exercises/prime_checker.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-fizzbuzz sol-number_guess sol-patterns sol-prime_checker

sol-fizzbuzz: solutions/fizzbuzz.c
	$(CC) $(CFLAGS) -o sol-fizzbuzz solutions/fizzbuzz.c

sol-number_guess: solutions/number_guess.c
	$(CC) $(CFLAGS) -o sol-number_guess solutions/number_guess.c

sol-patterns: solutions/patterns.c
	$(CC) $(CFLAGS) -o sol-patterns solutions/patterns.c

sol-prime_checker: solutions/prime_checker.c
	$(CC) $(CFLAGS) -o sol-prime_checker solutions/prime_checker.c

# --- Run targets ---

run-fizzbuzz: fizzbuzz
	./fizzbuzz

run-patterns: patterns
	./patterns

run-prime_checker: prime_checker
	./prime_checker

run-number_guess: number_guess
	./number_guess

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- FizzBuzz (first 16 lines) ---"
	./sol-fizzbuzz | head -16
	@echo ""
	@echo "--- Patterns ---"
	./sol-patterns
	@echo ""
	@echo "--- Prime Checker ---"
	./sol-prime_checker
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f fizzbuzz number_guess patterns prime_checker
	rm -f sol-fizzbuzz sol-number_guess sol-patterns sol-prime_checker

.PHONY: exercises solutions test clean
.PHONY: run-fizzbuzz run-number_guess run-patterns run-prime_checker
