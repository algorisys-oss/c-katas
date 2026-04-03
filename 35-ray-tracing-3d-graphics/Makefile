CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99
LDFLAGS = -lm

# --- Exercise targets ---

exercises: vec3 raytracer

vec3: exercises/vec3.c
	$(CC) $(CFLAGS) -o vec3 exercises/vec3.c $(LDFLAGS)

raytracer: exercises/raytracer.c
	$(CC) $(CFLAGS) -o raytracer exercises/raytracer.c $(LDFLAGS)

# --- Solution targets (prefix with sol-) ---

solutions: sol-vec3 sol-raytracer

sol-vec3: solutions/vec3.c
	$(CC) $(CFLAGS) -o sol-vec3 solutions/vec3.c $(LDFLAGS)

sol-raytracer: solutions/raytracer.c
	$(CC) $(CFLAGS) -o sol-raytracer solutions/raytracer.c $(LDFLAGS)

# --- Test: build and run solutions to verify correctness ---

test: solutions
	@echo "--- Vec3 Library ---"
	./sol-vec3
	@echo ""
	@echo "--- Ray Tracer ---"
	./sol-raytracer
	@echo ""
	@echo "All solution tests complete."

# --- Clean up ---

clean:
	rm -f vec3 raytracer
	rm -f sol-vec3 sol-raytracer

.PHONY: exercises solutions test clean
