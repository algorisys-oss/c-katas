CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
LDFLAGS = -lm

# --- Exercise targets ---

exercises: image_ops

image_ops: exercises/image_ops.c
	$(CC) $(CFLAGS) -o image_ops exercises/image_ops.c $(LDFLAGS)

# --- Solution targets (prefix with sol-) ---

solutions: sol-image_ops

sol-image_ops: solutions/image_ops.c
	$(CC) $(CFLAGS) -o sol-image_ops solutions/image_ops.c $(LDFLAGS)

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Image Manipulation & Effects ---"
	./sol-image_ops
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f image_ops sol-image_ops

.PHONY: exercises solutions test clean
