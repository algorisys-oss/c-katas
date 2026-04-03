CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: stack queue expression_eval monotonic_stack decode_string design_problems

stack: exercises/stack.c
	$(CC) $(CFLAGS) -o stack exercises/stack.c

queue: exercises/queue.c
	$(CC) $(CFLAGS) -o queue exercises/queue.c

expression_eval: exercises/expression_eval.c
	$(CC) $(CFLAGS) -o expression_eval exercises/expression_eval.c

monotonic_stack: exercises/monotonic_stack.c
	$(CC) $(CFLAGS) -o monotonic_stack exercises/monotonic_stack.c

decode_string: exercises/decode_string.c
	$(CC) $(CFLAGS) -o decode_string exercises/decode_string.c

design_problems: exercises/design_problems.c
	$(CC) $(CFLAGS) -o design_problems exercises/design_problems.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-stack sol-queue sol-expression_eval sol-monotonic_stack sol-decode_string sol-design_problems

sol-stack: solutions/stack.c
	$(CC) $(CFLAGS) -o sol-stack solutions/stack.c

sol-queue: solutions/queue.c
	$(CC) $(CFLAGS) -o sol-queue solutions/queue.c

sol-expression_eval: solutions/expression_eval.c
	$(CC) $(CFLAGS) -o sol-expression_eval solutions/expression_eval.c

sol-monotonic_stack: solutions/monotonic_stack.c
	$(CC) $(CFLAGS) -o sol-monotonic_stack solutions/monotonic_stack.c

sol-decode_string: solutions/decode_string.c
	$(CC) $(CFLAGS) -o sol-decode_string solutions/decode_string.c

sol-design_problems: solutions/design_problems.c
	$(CC) $(CFLAGS) -o sol-design_problems solutions/design_problems.c

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
	@echo "--- Monotonic Stack ---"
	./sol-monotonic_stack
	@echo ""
	@echo "--- Decode String ---"
	./sol-decode_string
	@echo ""
	@echo "--- Design Problems ---"
	./sol-design_problems
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f stack queue expression_eval monotonic_stack decode_string design_problems
	rm -f sol-stack sol-queue sol-expression_eval sol-monotonic_stack sol-decode_string sol-design_problems

.PHONY: exercises solutions test clean
