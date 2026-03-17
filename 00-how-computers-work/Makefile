CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -std=c99

EX_DIR  = exercises
SOL_DIR = solutions

EX_SRCS  = $(wildcard $(EX_DIR)/*.c)
SOL_SRCS = $(wildcard $(SOL_DIR)/*.c)

EX_BINS  = $(EX_SRCS:.c=)
SOL_BINS = $(SOL_SRCS:.c=)

.PHONY: all exercises solutions test clean

all: exercises solutions

exercises: $(EX_BINS)

solutions: $(SOL_BINS)

$(EX_DIR)/%: $(EX_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ $<

$(SOL_DIR)/%: $(SOL_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ $<

test: solutions
	@echo "=== Running base_converter solution ==="
	@$(SOL_DIR)/base_converter
	@echo ""
	@echo "=== Running binary_printer solution ==="
	@$(SOL_DIR)/binary_printer

clean:
	rm -f $(EX_BINS) $(SOL_BINS)
