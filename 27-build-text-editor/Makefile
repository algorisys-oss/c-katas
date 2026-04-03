CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: gap_buffer screen_buffer

gap_buffer: exercises/gap_buffer.c
	$(CC) $(CFLAGS) -o gap_buffer exercises/gap_buffer.c

screen_buffer: exercises/screen_buffer.c
	$(CC) $(CFLAGS) -o screen_buffer exercises/screen_buffer.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-gap_buffer sol-screen_buffer

sol-gap_buffer: solutions/gap_buffer.c
	$(CC) $(CFLAGS) -o sol-gap_buffer solutions/gap_buffer.c

sol-screen_buffer: solutions/screen_buffer.c
	$(CC) $(CFLAGS) -o sol-screen_buffer solutions/screen_buffer.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Gap Buffer ---"
	./sol-gap_buffer
	@echo ""
	@echo "--- Screen Buffer ---"
	./sol-screen_buffer
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f gap_buffer screen_buffer
	rm -f sol-gap_buffer sol-screen_buffer

.PHONY: exercises solutions test clean
