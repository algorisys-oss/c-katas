CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: generic_sort callbacks command_dispatch

generic_sort: exercises/generic_sort.c
	$(CC) $(CFLAGS) -o generic_sort exercises/generic_sort.c

callbacks: exercises/callbacks.c
	$(CC) $(CFLAGS) -o callbacks exercises/callbacks.c

command_dispatch: exercises/command_dispatch.c
	$(CC) $(CFLAGS) -o command_dispatch exercises/command_dispatch.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-generic_sort sol-callbacks sol-command_dispatch

sol-generic_sort: solutions/generic_sort.c
	$(CC) $(CFLAGS) -o sol-generic_sort solutions/generic_sort.c

sol-callbacks: solutions/callbacks.c
	$(CC) $(CFLAGS) -o sol-callbacks solutions/callbacks.c

sol-command_dispatch: solutions/command_dispatch.c
	$(CC) $(CFLAGS) -o sol-command_dispatch solutions/command_dispatch.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Generic Sort ---"
	./sol-generic_sort
	@echo ""
	@echo "--- Event System ---"
	./sol-callbacks
	@echo ""
	@echo "--- Command Dispatcher ---"
	./sol-command_dispatch
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f generic_sort callbacks command_dispatch
	rm -f sol-generic_sort sol-callbacks sol-command_dispatch

.PHONY: exercises solutions test clean
