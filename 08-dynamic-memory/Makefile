CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: string_builder dynamic_array leak_detective

string_builder: exercises/string_builder.c
	$(CC) $(CFLAGS) -o string_builder exercises/string_builder.c

dynamic_array: exercises/dynamic_array.c
	$(CC) $(CFLAGS) -o dynamic_array exercises/dynamic_array.c

leak_detective: exercises/leak_detective.c
	$(CC) $(CFLAGS) -o leak_detective exercises/leak_detective.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-string_builder sol-dynamic_array sol-leak_detective

sol-string_builder: solutions/string_builder.c
	$(CC) $(CFLAGS) -o sol-string_builder solutions/string_builder.c

sol-dynamic_array: solutions/dynamic_array.c
	$(CC) $(CFLAGS) -o sol-dynamic_array solutions/dynamic_array.c

sol-leak_detective: solutions/leak_detective.c
	$(CC) $(CFLAGS) -o sol-leak_detective solutions/leak_detective.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- String Builder ---"
	./sol-string_builder
	@echo ""
	@echo "--- Dynamic Array ---"
	./sol-dynamic_array
	@echo ""
	@echo "--- Leak Detective ---"
	./sol-leak_detective
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f string_builder dynamic_array leak_detective
	rm -f sol-string_builder sol-dynamic_array sol-leak_detective

.PHONY: exercises solutions test clean
