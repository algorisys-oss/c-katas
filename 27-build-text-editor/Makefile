CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: gap_buffer screen_buffer terminal input editor

gap_buffer: exercises/gap_buffer.c
	$(CC) $(CFLAGS) -o gap_buffer exercises/gap_buffer.c

screen_buffer: exercises/screen_buffer.c
	$(CC) $(CFLAGS) -o screen_buffer exercises/screen_buffer.c

terminal: exercises/terminal.c
	$(CC) $(CFLAGS) -o terminal exercises/terminal.c

input: exercises/input.c
	$(CC) $(CFLAGS) -o input exercises/input.c

editor: exercises/editor.c
	$(CC) $(CFLAGS) -o editor exercises/editor.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-gap_buffer sol-screen_buffer sol-terminal sol-input sol-editor

sol-gap_buffer: solutions/gap_buffer.c
	$(CC) $(CFLAGS) -o sol-gap_buffer solutions/gap_buffer.c

sol-screen_buffer: solutions/screen_buffer.c
	$(CC) $(CFLAGS) -o sol-screen_buffer solutions/screen_buffer.c

sol-terminal: solutions/terminal.c
	$(CC) $(CFLAGS) -o sol-terminal solutions/terminal.c

sol-input: solutions/input.c
	$(CC) $(CFLAGS) -o sol-input solutions/input.c

sol-editor: solutions/editor.c
	$(CC) $(CFLAGS) -o sol-editor solutions/editor.c

# --- Test: build and run solutions to verify correctness ---

test: sol-gap_buffer sol-screen_buffer sol-terminal sol-input
	@echo "--- Gap Buffer ---"
	./sol-gap_buffer
	@echo ""
	@echo "--- Screen Buffer ---"
	./sol-screen_buffer
	@echo ""
	@echo "--- Terminal Control ---"
	./sol-terminal
	@echo ""
	@echo "--- Keyboard Input ---"
	./sol-input
	@echo ""
	@echo "All solution tests complete."

# --- Run: build and launch the editor solution ---

run: sol-editor
	./sol-editor

# --- Clean up ---

clean:
	rm -f gap_buffer screen_buffer terminal input editor
	rm -f sol-gap_buffer sol-screen_buffer sol-terminal sol-input sol-editor

.PHONY: exercises solutions test run clean
