SUBDIRS = exercises solutions

.PHONY: all clean test $(SUBDIRS)

all: exercises

solutions:
	$(MAKE) -C solutions

exercises:
	$(MAKE) -C exercises

test: exercises
	@echo "=== Arena ===" && cd exercises && ./arena || true
	@echo "" && echo "=== Pool ===" && cd exercises && ./pool || true
	@echo "" && echo "=== Stack Alloc ===" && cd exercises && ./stack_alloc || true

clean:
	$(MAKE) -C exercises clean
	@if [ -f solutions/Makefile ]; then $(MAKE) -C solutions clean; fi
