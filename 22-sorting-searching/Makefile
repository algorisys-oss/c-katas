CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: sorting binary_search two_pointers sliding_window merge_intervals string_problems modified_binary_search

sorting: exercises/sorting.c
	$(CC) $(CFLAGS) -o sorting exercises/sorting.c

binary_search: exercises/binary_search.c
	$(CC) $(CFLAGS) -o binary_search exercises/binary_search.c

two_pointers: exercises/two_pointers.c
	$(CC) $(CFLAGS) -o two_pointers exercises/two_pointers.c

sliding_window: exercises/sliding_window.c
	$(CC) $(CFLAGS) -o sliding_window exercises/sliding_window.c

merge_intervals: exercises/merge_intervals.c
	$(CC) $(CFLAGS) -o merge_intervals exercises/merge_intervals.c

string_problems: exercises/string_problems.c
	$(CC) $(CFLAGS) -o string_problems exercises/string_problems.c

modified_binary_search: exercises/modified_binary_search.c
	$(CC) $(CFLAGS) -o modified_binary_search exercises/modified_binary_search.c

# --- Solution targets (prefix with sol-) ---

solutions: sol-sorting sol-binary_search sol-two_pointers sol-sliding_window sol-merge_intervals sol-string_problems sol-modified_binary_search

sol-sorting: solutions/sorting.c
	$(CC) $(CFLAGS) -o sol-sorting solutions/sorting.c

sol-binary_search: solutions/binary_search.c
	$(CC) $(CFLAGS) -o sol-binary_search solutions/binary_search.c

sol-two_pointers: solutions/two_pointers.c
	$(CC) $(CFLAGS) -o sol-two_pointers solutions/two_pointers.c

sol-sliding_window: solutions/sliding_window.c
	$(CC) $(CFLAGS) -o sol-sliding_window solutions/sliding_window.c

sol-merge_intervals: solutions/merge_intervals.c
	$(CC) $(CFLAGS) -o sol-merge_intervals solutions/merge_intervals.c

sol-string_problems: solutions/string_problems.c
	$(CC) $(CFLAGS) -o sol-string_problems solutions/string_problems.c

sol-modified_binary_search: solutions/modified_binary_search.c
	$(CC) $(CFLAGS) -o sol-modified_binary_search solutions/modified_binary_search.c

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Sorting Algorithms ---"
	./sol-sorting
	@echo ""
	@echo "--- Binary Search ---"
	./sol-binary_search
	@echo ""
	@echo "--- Two Pointers ---"
	./sol-two_pointers
	@echo ""
	@echo "--- Sliding Window ---"
	./sol-sliding_window
	@echo ""
	@echo "--- Merge Intervals ---"
	./sol-merge_intervals
	@echo ""
	@echo "--- String Problems ---"
	./sol-string_problems
	@echo ""
	@echo "--- Modified Binary Search ---"
	./sol-modified_binary_search
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f sorting binary_search two_pointers sliding_window merge_intervals string_problems modified_binary_search
	rm -f sol-sorting sol-binary_search sol-two_pointers sol-sliding_window sol-merge_intervals sol-string_problems sol-modified_binary_search

.PHONY: exercises solutions test clean
