CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
LDFLAGS = -lm

# --- Exercise targets ---

exercises: matrix neural_net

matrix: exercises/matrix.c
	$(CC) $(CFLAGS) -o matrix exercises/matrix.c $(LDFLAGS)

neural_net: exercises/neural_net.c
	$(CC) $(CFLAGS) -o neural_net exercises/neural_net.c $(LDFLAGS)

# --- Solution targets (prefix with sol-) ---

solutions: sol-matrix sol-neural_net

sol-matrix: solutions/matrix.c
	$(CC) $(CFLAGS) -o sol-matrix solutions/matrix.c $(LDFLAGS)

sol-neural_net: solutions/neural_net.c
	$(CC) $(CFLAGS) -o sol-neural_net solutions/neural_net.c $(LDFLAGS)

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Matrix Math Library ---"
	./sol-matrix
	@echo ""
	@echo "--- Neural Network ---"
	./sol-neural_net
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f matrix neural_net
	rm -f sol-matrix sol-neural_net

.PHONY: exercises solutions test clean
