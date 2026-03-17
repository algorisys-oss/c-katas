/*
 * Kata: Number Guessing Game
 *
 * The program picks a random number between 1 and 100.
 * The player guesses until they get it right.
 * After each guess, tell them "Too high!" or "Too low!"
 * When they guess correctly, print how many attempts it took.
 *
 * Example session:
 *   I'm thinking of a number between 1 and 100.
 *   Your guess: 50
 *   Too high!
 *   Your guess: 25
 *   Too low!
 *   Your guess: 37
 *   Correct! You got it in 3 attempts.
 *
 * Concepts used:
 *   - rand() and srand() for random numbers
 *   - scanf() for reading input
 *   - while loop (or do-while)
 *   - if/else
 *   - break
 *
 * Hints:
 *   - srand(time(NULL)) seeds the random number generator
 *   - rand() % 100 + 1 gives a number from 1 to 100
 *   - scanf("%d", &guess) reads an integer into guess
 *   - Use a counter variable to track the number of attempts
 *   - A while(1) loop with a break works well here
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    /* TODO: Seed the random number generator with srand(time(NULL)) */

    /* TODO: Generate a random number between 1 and 100 (inclusive)
     *       Use: rand() % 100 + 1
     *       Store it in a variable called 'secret'
     */

    /* TODO: Declare a variable 'guess' for the player's guess */

    /* TODO: Declare a variable 'attempts' and initialize it to 0 */

    printf("I'm thinking of a number between 1 and 100.\n");

    /* TODO: Write a loop that:
     *   1. Prints "Your guess: "
     *   2. Reads the player's guess with scanf("%d", &guess)
     *   3. Increments the attempts counter
     *   4. If guess == secret, print "Correct! You got it in %d attempts.\n"
     *      and break out of the loop
     *   5. If guess > secret, print "Too high!\n"
     *   6. If guess < secret, print "Too low!\n"
     *
     * Hint: a while(1) loop with break is a clean approach here
     */

    return 0;
}
