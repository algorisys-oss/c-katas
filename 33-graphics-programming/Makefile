CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

# --- Exercise targets ---

exercises: ppm shapes mandelbrot

ppm: exercises/ppm.c
	$(CC) $(CFLAGS) -o ppm exercises/ppm.c

shapes: exercises/shapes.c
	$(CC) $(CFLAGS) -o shapes exercises/shapes.c

mandelbrot: exercises/mandelbrot.c
	$(CC) $(CFLAGS) -o mandelbrot exercises/mandelbrot.c -lm

# --- Solution targets (prefix with sol-) ---

solutions: sol-ppm sol-shapes sol-mandelbrot

sol-ppm: solutions/ppm.c
	$(CC) $(CFLAGS) -o sol-ppm solutions/ppm.c

sol-shapes: solutions/shapes.c
	$(CC) $(CFLAGS) -o sol-shapes solutions/shapes.c

sol-mandelbrot: solutions/mandelbrot.c
	$(CC) $(CFLAGS) -o sol-mandelbrot solutions/mandelbrot.c -lm

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- PPM Image ---"
	./sol-ppm
	@echo ""
	@echo "--- Drawing Shapes ---"
	./sol-shapes
	@echo ""
	@echo "--- Mandelbrot Set ---"
	./sol-mandelbrot
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f ppm shapes mandelbrot sol-ppm sol-shapes sol-mandelbrot
	rm -f /tmp/test_ppm_output.ppm /tmp/test_ppm_size.ppm
	rm -f /tmp/shapes_test.ppm /tmp/mandelbrot_test.ppm

.PHONY: exercises solutions test clean
