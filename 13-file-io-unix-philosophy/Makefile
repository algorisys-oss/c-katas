CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: hexdump wc todo_app

hexdump: exercises/hexdump.c
	$(CC) $(CFLAGS) -o hexdump exercises/hexdump.c

wc: exercises/wc.c
	$(CC) $(CFLAGS) -o wc exercises/wc.c

todo_app: exercises/todo_app.c
	$(CC) $(CFLAGS) -o todo_app exercises/todo_app.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-hexdump sol-wc sol-todo_app

sol-hexdump: solutions/hexdump.c
	$(CC) $(CFLAGS) -o sol-hexdump solutions/hexdump.c

sol-wc: solutions/wc.c
	$(CC) $(CFLAGS) -o sol-wc solutions/wc.c

sol-todo_app: solutions/todo_app.c
	$(CC) $(CFLAGS) -o sol-todo_app solutions/todo_app.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Hex Dump ---"
	./sol-hexdump
	@echo ""
	@echo "--- wc (Word Count) ---"
	./sol-wc
	@echo ""
	@echo "--- Todo App ---"
	./sol-todo_app
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f hexdump wc todo_app
	rm -f sol-hexdump sol-wc sol-todo_app

.PHONY: exercises solutions test clean
