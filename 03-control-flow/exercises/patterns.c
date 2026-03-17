/*
 * Kata: Pattern Printing
 *
 * Print shapes using nested loops. This exercises your understanding
 * of how inner and outer loops interact.
 *
 * You will implement four functions, each printing a different pattern.
 * The main function calls each one so you can see all patterns at once.
 *
 * Hints:
 *   - The outer loop controls ROWS
 *   - The inner loop controls what you print in EACH row
 *   - For spaces and stars, think about how many of each per row
 *   - Trace through a small example (n=3) by hand first!
 */

#include <stdio.h>

/*
 * Pattern 1: Right-aligned triangle (height n)
 *
 * Example for n = 5:
 *         *
 *        **
 *       ***
 *      ****
 *     *****
 *
 * Row 1: 4 spaces, 1 star
 * Row 2: 3 spaces, 2 stars
 * Row i: (n-i) spaces, i stars
 */
void right_triangle(int n)
{
    printf("=== Right Triangle (n=%d) ===\n", n);
    /* TODO: Use nested loops to print the right-aligned triangle.
     *   Outer loop: row from 1 to n
     *     Inner loop 1: print (n - row) spaces
     *     Inner loop 2: print (row) stars
     *     Print a newline after each row
     */
}

/*
 * Pattern 2: Pyramid (height n)
 *
 * Example for n = 5:
 *         *
 *        ***
 *       *****
 *      *******
 *     *********
 *
 * Row 1: 4 spaces, 1 star
 * Row 2: 3 spaces, 3 stars
 * Row i: (n-i) spaces, (2*i - 1) stars
 */
void pyramid(int n)
{
    printf("=== Pyramid (n=%d) ===\n", n);
    /* TODO: Use nested loops to print the pyramid.
     *   Outer loop: row from 1 to n
     *     Inner loop 1: print (n - row) spaces
     *     Inner loop 2: print (2 * row - 1) stars
     *     Print a newline after each row
     */
}

/*
 * Pattern 3: Diamond (height 2*n - 1)
 *
 * Example for n = 5:
 *         *
 *        ***
 *       *****
 *      *******
 *     *********
 *      *******
 *       *****
 *        ***
 *         *
 *
 * Top half: same as the pyramid (rows 1 to n)
 * Bottom half: pyramid in reverse (rows n-1 down to 1)
 */
void diamond(int n)
{
    printf("=== Diamond (n=%d) ===\n", n);
    /* TODO: Print the top half (rows 1 to n).
     *   This is the same as the pyramid above.
     */

    /* TODO: Print the bottom half (rows n-1 down to 1).
     *   Same logic as the top half, but the row counter
     *   goes from (n-1) down to 1.
     */
}

/*
 * Pattern 4: Multiplication table (n x n)
 *
 * Example for n = 5:
 *       1    2    3    4    5
 *       2    4    6    8   10
 *       3    6    9   12   15
 *       4    8   12   16   20
 *       5   10   15   20   25
 *
 * Each cell is row * col, printed with width 5.
 */
void multiplication_table(int n)
{
    printf("=== Multiplication Table (%dx%d) ===\n", n, n);
    /* TODO: Use nested loops to print the multiplication table.
     *   Outer loop: row from 1 to n
     *     Inner loop: col from 1 to n
     *       Print row * col with printf("%5d", row * col)
     *     Print a newline after each row
     */
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
