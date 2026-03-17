/*
 * Kata: FizzBuzz
 *
 * Print the numbers from 1 to 100, but:
 *   - If the number is divisible by 3, print "Fizz" instead
 *   - If the number is divisible by 5, print "Buzz" instead
 *   - If the number is divisible by both 3 and 5, print "FizzBuzz" instead
 *   - Otherwise, print the number itself
 *
 * Each output on its own line.
 *
 * Expected output (first 16 lines):
 *   1
 *   2
 *   Fizz
 *   4
 *   Buzz
 *   Fizz
 *   7
 *   8
 *   Fizz
 *   Buzz
 *   11
 *   Fizz
 *   13
 *   14
 *   FizzBuzz
 *   16
 *
 * Hints:
 *   - Use the modulo operator (%) to check divisibility
 *   - a % b == 0 means "a is divisible by b"
 *   - Check divisibility by 15 (both 3 and 5) FIRST — why?
 *     Because if you check 3 first, you will print "Fizz" and
 *     never reach the "both" check.
 *   - Use a for loop from 1 to 100
 */

#include <stdio.h>

int main(void)
{
    /* TODO: Write a for loop from 1 to 100 (inclusive) */
    /* TODO: Inside the loop, use if/else if/else to check:
     *   1. divisible by both 3 and 5 -> print "FizzBuzz"
     *   2. divisible by 3           -> print "Fizz"
     *   3. divisible by 5           -> print "Buzz"
     *   4. otherwise                -> print the number
     */

    return 0;
}
