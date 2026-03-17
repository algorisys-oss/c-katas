#include <stdio.h>

int main(void)
{
    const char *name = "Alice";
    int age = 17;
    double height = 1.65;
    int favorite_number = 42;

    printf("Name: %s\n", name);
    printf("Age: %d\n", age);
    printf("Height: %.2f meters\n", height);
    printf("Favorite number: %d (hex: %x)\n", favorite_number, favorite_number);

    return 0;
}
