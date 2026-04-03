SUBDIRS = exercises solutions

.PHONY: all clean test $(SUBDIRS)

all: exercises

solutions:
	$(MAKE) -C solutions

exercises:
	$(MAKE) -C exercises

test: exercises
	@echo "=== GDB Detective ===" && cd exercises && ./gdb_detective || true
	@echo "" && echo "=== Valgrind Hunt ===" && cd exercises && ./valgrind_hunt || true
	@echo "" && echo "=== Segfault Safari ===" && cd exercises && ./segfault_safari || true

clean:
	$(MAKE) -C exercises clean
	@if [ -f solutions/Makefile ]; then $(MAKE) -C solutions clean; fi
