CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: stack queue expression_eval

stack: exercises/stack.c
	$(CC) $(CFLAGS) -o stack exercises/stack.c

queue: exercises/queue.c
	$(CC) $(CFLAGS) -o queue exercises/queue.c

expression_eval: exercises/expression_eval.c
	$(CC) $(CFLAGS) -o expression_eval exercises/expression_eval.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-stack sol-queue sol-expression_eval

sol-stack: solutions/stack.c
	$(CC) $(CFLAGS) -o sol-stack solutions/stack.c

sol-queue: solutions/queue.c
	$(CC) $(CFLAGS) -o sol-queue solutions/queue.c

sol-expression_eval: solutions/expression_eval.c
	$(CC) $(CFLAGS) -o sol-expression_eval solutions/expression_eval.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Stack & Balanced Parentheses ---"
	./sol-stack
	@echo ""
	@echo "--- Circular Buffer Queue ---"
	./sol-queue
	@echo ""
	@echo "--- Expression Evaluator ---"
	./sol-expression_eval
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f stack queue expression_eval
	rm -f sol-stack sol-queue sol-expression_eval

.PHONY: exercises solutions test clean
