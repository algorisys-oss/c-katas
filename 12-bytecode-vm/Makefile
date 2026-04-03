SUBDIRS = exercises solutions

.PHONY: all clean test $(SUBDIRS)

all: exercises

solutions:
	$(MAKE) -C solutions

exercises:
	$(MAKE) -C exercises

test: exercises
	@echo "=== VM ===" && cd exercises && ./vm || true
	@echo "" && echo "=== Disassembler ===" && cd exercises && ./disassembler || true

clean:
	$(MAKE) -C exercises clean
	@if [ -f solutions/Makefile ]; then $(MAKE) -C solutions clean; fi
