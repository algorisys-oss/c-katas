#include <stdio.h>

/*
 * Right-aligned triangle
 */
void right_triangle(int n)
{
    printf("=== Right Triangle (n=%d) ===\n", n);
    for (int row = 1; row <= n; row++) {
        for (int s = 0; s < n - row; s++) {
            printf(" ");
        }
        for (int star = 0; star < row; star++) {
            printf("*");
        }
        printf("\n");
    }
}

/*
 * Pyramid
 */
void pyramid(int n)
{
    printf("=== Pyramid (n=%d) ===\n", n);
    for (int row = 1; row <= n; row++) {
        for (int s = 0; s < n - row; s++) {
            printf(" ");
        }
        for (int star = 0; star < 2 * row - 1; star++) {
            printf("*");
        }
        printf("\n");
    }
}

/*
 * Diamond
 */
void diamond(int n)
{
    printf("=== Diamond (n=%d) ===\n", n);

    /* Top half (rows 1 to n) */
    for (int row = 1; row <= n; row++) {
        for (int s = 0; s < n - row; s++) {
            printf(" ");
        }
        for (int star = 0; star < 2 * row - 1; star++) {
            printf("*");
        }
        printf("\n");
    }

    /* Bottom half (rows n-1 down to 1) */
    for (int row = n - 1; row >= 1; row--) {
        for (int s = 0; s < n - row; s++) {
            printf(" ");
        }
        for (int star = 0; star < 2 * row - 1; star++) {
            printf("*");
        }
        printf("\n");
    }
}

/*
 * Multiplication table
 */
void multiplication_table(int n)
{
    printf("=== Multiplication Table (%dx%d) ===\n", n, n);
    for (int row = 1; row <= n; row++) {
        for (int col = 1; col <= n; col++) {
            printf("%5d", row * col);
        }
        printf("\n");
    }
}

int main(void)
{
    right_triangle(5);
    printf("\n");

    pyramid(5);
    printf("\n");

    diamond(5);
    printf("\n");

    multiplication_table(5);

    return 0;
}
