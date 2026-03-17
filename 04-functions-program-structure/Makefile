CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: calculator temperature math_lib_test

calculator: exercises/calculator.c
	$(CC) $(CFLAGS) -o calculator exercises/calculator.c

temperature: exercises/temperature.c
	$(CC) $(CFLAGS) -o temperature exercises/temperature.c

math_lib_test: exercises/math_lib.c exercises/math_lib_test.c exercises/math_lib.h
	$(CC) $(CFLAGS) -o math_lib_test exercises/math_lib.c exercises/math_lib_test.c -lm

# --- Solution targets (prefix with sol-) ---

solutions: sol-calculator sol-temperature sol-math_lib_test

sol-calculator: solutions/calculator.c
	$(CC) $(CFLAGS) -o sol-calculator solutions/calculator.c

sol-temperature: solutions/temperature.c
	$(CC) $(CFLAGS) -o sol-temperature solutions/temperature.c

sol-math_lib_test: solutions/math_lib.c solutions/math_lib_test.c solutions/math_lib.h
	$(CC) $(CFLAGS) -o sol-math_lib_test solutions/math_lib.c solutions/math_lib_test.c -lm

# --- Run targets ---

run-calculator: calculator
	./calculator

run-temperature: temperature
	./temperature

run-math_lib_test: math_lib_test
	./math_lib_test

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Temperature Converter ---"
	./sol-temperature
	@echo ""
	@echo "--- Math Library Tests ---"
	./sol-math_lib_test
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f calculator temperature math_lib_test
	rm -f sol-calculator sol-temperature sol-math_lib_test

.PHONY: exercises solutions test clean
.PHONY: run-calculator run-temperature run-math_lib_test
