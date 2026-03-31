CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: pointers strings arrays string_tokenizer

pointers: exercises/pointers.c
	$(CC) $(CFLAGS) -o pointers exercises/pointers.c

strings: exercises/strings.c
	$(CC) $(CFLAGS) -o strings exercises/strings.c

arrays: exercises/arrays.c
	$(CC) $(CFLAGS) -o arrays exercises/arrays.c

string_tokenizer: exercises/string_tokenizer.c
	$(CC) $(CFLAGS) -o string_tokenizer exercises/string_tokenizer.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-pointers sol-strings sol-arrays sol-string_tokenizer

sol-pointers: solutions/pointers.c
	$(CC) $(CFLAGS) -o sol-pointers solutions/pointers.c

sol-strings: solutions/strings.c
	$(CC) $(CFLAGS) -o sol-strings solutions/strings.c

sol-arrays: solutions/arrays.c
	$(CC) $(CFLAGS) -o sol-arrays solutions/arrays.c

sol-string_tokenizer: solutions/string_tokenizer.c
	$(CC) $(CFLAGS) -o sol-string_tokenizer solutions/string_tokenizer.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Pointer Basics ---"
	./sol-pointers
	@echo ""
	@echo "--- String Functions ---"
	./sol-strings
	@echo ""
	@echo "--- Array Operations ---"
	./sol-arrays
	@echo ""
	@echo "--- String Tokenizer ---"
	./sol-string_tokenizer
	@echo ""
	@echo "All solution tests complete."

# --- Debug exercise targets ---

debug: exercises/debug_pointers.c
	$(CC) $(CFLAGS) exercises/debug_pointers.c -o exercises/debug_pointers

debug-solution: solutions/debug_pointers.c
	$(CC) $(CFLAGS) solutions/debug_pointers.c -o solutions/debug_pointers

# --- Clean up ---

clean:
	rm -f pointers strings arrays string_tokenizer
	rm -f sol-pointers sol-strings sol-arrays sol-string_tokenizer
	rm -f exercises/debug_pointers solutions/debug_pointers

.PHONY: exercises solutions test clean debug debug-solution
