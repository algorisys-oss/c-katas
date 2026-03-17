/*
 * Kata: Tower of Hanoi
 *
 * The Tower of Hanoi is a classic recursion puzzle:
 *   - You have 3 pegs: A (source), B (auxiliary), C (destination)
 *   - There are n disks of different sizes on peg A, smallest on top
 *   - Move all disks from A to C, one at a time
 *   - You may never place a larger disk on top of a smaller one
 *
 * The recursive solution:
 *   1. Move n-1 disks from source to auxiliary (using destination as helper)
 *   2. Move the nth (largest) disk from source to destination
 *   3. Move n-1 disks from auxiliary to destination (using source as helper)
 *
 * Base case: n == 0 — nothing to move
 *
 * The function prints each move: "Move disk 1 from A to C"
 *
 * Expected output for hanoi(3, 'A', 'C', 'B'):
 *   Move disk 1 from A to C
 *   Move disk 2 from A to B
 *   Move disk 1 from C to B
 *   Move disk 3 from A to C
 *   Move disk 1 from B to A
 *   Move disk 2 from B to C
 *   Move disk 1 from A to C
 *
 * Total moves for n disks: 2^n - 1
 *
 * Hints:
 *   - The function takes: n (disks), source, destination, auxiliary (char)
 *   - Trust the recursion: if you can move n-1 disks, you can move n
 */

#include <stdio.h>

/* Print each move to solve Tower of Hanoi with n disks.
 * Move disks from 'source' to 'destination' using 'auxiliary'. */
void hanoi(int n, char source, char destination, char auxiliary)
{
    /* TODO: Base case — if n == 0, just return (nothing to move) */

    /* TODO: Step 1 — recursively move n-1 disks from source to auxiliary
     *       (destination acts as the helper peg) */

    /* TODO: Step 2 — print the move for the nth disk:
     *       printf("Move disk %d from %c to %c\n", n, source, destination); */

    /* TODO: Step 3 — recursively move n-1 disks from auxiliary to destination
     *       (source acts as the helper peg) */
}

/* ---- Test harness (do not modify) ---- */

int main(void)
{
    printf("=== Tower of Hanoi ===\n\n");

    for (int n = 1; n <= 4; n++) {
        printf("--- %d disk%s (expect %d moves) ---\n",
               n, n == 1 ? "" : "s", (1 << n) - 1);
        hanoi(n, 'A', 'C', 'B');
        printf("\n");
    }

    return 0;
}
