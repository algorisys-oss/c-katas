SUBDIRS = exercises solutions

.PHONY: all clean test $(SUBDIRS)

all: exercises

solutions:
	$(MAKE) -C solutions

exercises:
	$(MAKE) -C exercises

test: exercises
	$(MAKE) -C exercises test

clean:
	$(MAKE) -C exercises clean
	@if [ -f solutions/Makefile ]; then $(MAKE) -C solutions clean; fi
