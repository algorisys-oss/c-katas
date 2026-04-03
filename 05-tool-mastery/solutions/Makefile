# -- Module 05: Tool Mastery -- Solutions Makefile -------------------------
#
# Targets:
#   make all        -- build all exercises (default)
#   make debug      -- build all exercises with AddressSanitizer
#   make valgrind   -- run valgrind on valgrind_hunt
#   make clean      -- remove all binaries and object files
#
# Individual targets:
#   make gdb_detective
#   make valgrind_hunt
#   make segfault_safari

# -- Variables -------------------------------------------------------------

CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra -pedantic -g
SANFLAGS = -fsanitize=address

# List of all exercise binaries
TARGETS = gdb_detective valgrind_hunt segfault_safari

# -- Phony Targets ---------------------------------------------------------

.PHONY: all debug valgrind clean

# Default: build everything
all: $(TARGETS)

# Build with AddressSanitizer for extra checking
debug: CFLAGS += $(SANFLAGS)
debug: LDFLAGS += $(SANFLAGS)
debug: clean $(TARGETS)
	@echo ""
	@echo "Built with AddressSanitizer. Run any exercise to check:"
	@echo "  ./gdb_detective"
	@echo "  ./valgrind_hunt"
	@echo "  ./segfault_safari"

# Run valgrind on valgrind_hunt
valgrind: valgrind_hunt
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./valgrind_hunt

# Remove all build artifacts
clean:
	rm -f $(TARGETS) *.o

# -- Pattern Rule ----------------------------------------------------------
# Each binary is built from the matching .c file

%: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<
