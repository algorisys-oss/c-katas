#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    srand((unsigned int)time(NULL));

    int secret = rand() % 100 + 1;
    int guess = 0;
    int attempts = 0;

    printf("I'm thinking of a number between 1 and 100.\n");

    while (1) {
        printf("Your guess: ");
        if (scanf("%d", &guess) != 1) {
            printf("Invalid input. Enter a number.\n");
            while (getchar() != '\n') {} /* flush bad input */
            continue;
        }
        attempts++;

        if (guess == secret) {
            printf("Correct! You got it in %d attempts.\n", attempts);
            break;
        } else if (guess > secret) {
            printf("Too high!\n");
        } else {
            printf("Too low!\n");
        }
    }

    return 0;
}
