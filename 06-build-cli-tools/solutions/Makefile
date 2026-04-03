CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -g

TOOLS = mini_wc mini_grep mini_head mini_uniq

all: $(TOOLS)

mini_wc: mini_wc.c
	$(CC) $(CFLAGS) -o $@ $<

mini_grep: mini_grep.c
	$(CC) $(CFLAGS) -o $@ $<

mini_head: mini_head.c
	$(CC) $(CFLAGS) -o $@ $<

mini_uniq: mini_uniq.c
	$(CC) $(CFLAGS) -o $@ $<

test: all
	@echo "=== Running mini_wc tests ==="
	@./mini_wc --test || true
	@echo ""
	@echo "=== Running mini_grep tests ==="
	@./mini_grep --test || true
	@echo ""
	@echo "=== Running mini_head tests ==="
	@./mini_head --test || true
	@echo ""
	@echo "=== Running mini_uniq tests ==="
	@./mini_uniq --test || true
	@echo ""
	@echo "=== All test suites complete ==="

clean:
	rm -f $(TOOLS)

.PHONY: all test clean
