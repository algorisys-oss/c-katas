CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: tokenizer eval

tokenizer: exercises/tokenizer.c
	$(CC) $(CFLAGS) -o tokenizer exercises/tokenizer.c

eval: exercises/eval.c
	$(CC) $(CFLAGS) -o eval exercises/eval.c -lm

# --- Solution targets (prefix with sol-) ---

solutions: sol-tokenizer sol-eval

sol-tokenizer: solutions/tokenizer.c
	$(CC) $(CFLAGS) -o sol-tokenizer solutions/tokenizer.c

sol-eval: solutions/eval.c
	$(CC) $(CFLAGS) -o sol-eval solutions/eval.c -lm

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- SQL Tokenizer ---"
	./sol-tokenizer
	@echo ""
	@echo "--- Expression Evaluator ---"
	./sol-eval
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f tokenizer eval
	rm -f sol-tokenizer sol-eval

.PHONY: exercises solutions test clean
