CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra -pedantic -g

TARGETS = vm disassembler

all: $(TARGETS)

vm: vm.c
	$(CC) $(CFLAGS) -o $@ $<

disassembler: disassembler.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

.PHONY: all clean
