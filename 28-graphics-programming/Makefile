CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: ppm

ppm: exercises/ppm.c
	$(CC) $(CFLAGS) -o ppm exercises/ppm.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-ppm

sol-ppm: solutions/ppm.c
	$(CC) $(CFLAGS) -o sol-ppm solutions/ppm.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- PPM Image ---"
	./sol-ppm
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f ppm sol-ppm
	rm -f /tmp/test_ppm_output.ppm /tmp/test_ppm_size.ppm

.PHONY: exercises solutions test clean
