#include <stdio.h>

/* Print each move to solve Tower of Hanoi with n disks */
void hanoi(int n, char source, char destination, char auxiliary)
{
    if (n == 0) {
        return;
    }
    hanoi(n - 1, source, auxiliary, destination);
    printf("Move disk %d from %c to %c\n", n, source, destination);
    hanoi(n - 1, auxiliary, destination, source);
}

/* ---- Test harness ---- */

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
