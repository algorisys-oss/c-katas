/*
 * binary_printer.c — Print any integer in its binary representation
 *
 * YOUR MISSION:
 *   Fill in the functions marked with TODO to build a binary printer.
 *   This will teach you how numbers are stored as bits inside the machine.
 *
 * CONCEPTS YOU'LL USE:
 *   - Bitwise AND (&) to test individual bits
 *   - Bit shifting (>> and <<) to move bits around
 *   - The sizeof operator to find how many bytes a type uses
 *   - CHAR_BIT from <limits.h> to know bits per byte (it's 8 on most systems)
 *
 * COMPILE:
 *   gcc -Wall -Wextra -pedantic -std=c99 -o binary_printer binary_printer.c
 *
 * RUN:
 *   ./binary_printer
 */

#include <stdio.h>
#include <limits.h>  /* for CHAR_BIT */

/*
 * print_binary_basic
 *
 * Print the binary representation of an unsigned 8-bit value (0-255).
 * Always prints exactly 8 bits (with leading zeros).
 *
 * Parameters:
 *   value - an unsigned char (0 to 255)
 *
 * Example output:
 *   print_binary_basic(42)  prints:  00101010
 *   print_binary_basic(255) prints:  11111111
 *   print_binary_basic(0)   prints:  00000000
 *
 * Algorithm:
 *   Loop from bit 7 (the leftmost/most significant) down to bit 0.
 *   For each bit position, check if that bit is set in the value.
 *   Print '1' if set, '0' if not.
 *
 * Hint: To check if bit N is set in a value:
 *       (value >> N) & 1
 *   This shifts the value right by N positions, then masks off everything
 *   except the last bit. The result is 0 or 1.
 *
 *   Alternatively:
 *       value & (1 << N)
 *   This creates a "mask" with only bit N set, then ANDs it with the value.
 *   The result is 0 or non-zero.
 */
void print_binary_basic(unsigned char value)
{
    /* TODO: Loop from bit 7 down to bit 0.
     *       For each position, print '1' or '0' depending on whether
     *       that bit is set in value. */

    printf("\n");
}

/*
 * print_binary_32
 *
 * Print the binary representation of a 32-bit unsigned integer.
 * Prints all 32 bits, grouped in sets of 4 for readability (with spaces).
 *
 * Parameters:
 *   value - an unsigned 32-bit integer
 *
 * Example output:
 *   print_binary_32(42)  prints:  0000 0000 0000 0000 0000 0000 0010 1010
 *   print_binary_32(0)   prints:  0000 0000 0000 0000 0000 0000 0000 0000
 *
 * Hint: Print a space after every 4 bits (except after the last group).
 *       You can check: if ((31 - i) % 4 == 3 && i != 0) print a space
 *       ...or come up with your own grouping logic.
 */
void print_binary_32(unsigned int value)
{
    int total_bits = 32;

    /* TODO: Loop from the most significant bit (bit 31) down to bit 0.
     *       For each position, print '1' or '0'.
     *       Insert a space every 4 bits for readability. */

    (void)total_bits;  /* remove this line once you use total_bits */
    printf("\n");
}

/*
 * print_binary_any_size
 *
 * Print the binary representation of any integer, regardless of its size.
 * This function takes a pointer to the data and its size in bytes.
 *
 * Parameters:
 *   data  - pointer to the value (we use void* so it works with any type)
 *   size  - number of bytes (use sizeof to get this)
 *
 * Example usage:
 *   int x = 42;
 *   print_binary_any_size(&x, sizeof(x));
 *
 * This is a STRETCH GOAL — it's trickier because you need to:
 *   1. Cast the void* to an unsigned char* to read individual bytes
 *   2. Process bytes from most significant to least significant
 *   3. For each byte, print its 8 bits
 *
 * Note: On most modern systems, bytes are stored in "little-endian" order,
 *       meaning the least significant byte comes first in memory. To print
 *       in the natural order (most significant first), you'll want to start
 *       from the last byte and work backward.
 *
 * Hint:
 *   unsigned char *bytes = (unsigned char *)data;
 *   Then bytes[0] is the first byte in memory, bytes[size-1] is the last.
 *   On a little-endian system, bytes[size-1] is the most significant byte.
 */
void print_binary_any_size(const void *data, size_t size)
{
    (void)data;  /* remove these lines once you use the parameters */
    (void)size;

    /* TODO (STRETCH GOAL):
     *   1. Cast data to (const unsigned char *)
     *   2. Loop through bytes from index (size-1) down to 0
     *   3. For each byte, loop through bits 7 down to 0
     *   4. Print '1' or '0' for each bit
     *   5. Print a space between bytes for readability */

    printf("\n");
}


/* ======================================================================
 * TEST HARNESS — DO NOT MODIFY BELOW THIS LINE
 * ====================================================================== */

int main(void)
{
    printf("=== 8-bit Binary Printer ===\n");
    printf("  0:   ");
    print_binary_basic(0);
    printf("  1:   ");
    print_binary_basic(1);
    printf("  42:  ");
    print_binary_basic(42);
    printf("  127: ");
    print_binary_basic(127);
    printf("  128: ");
    print_binary_basic(128);
    printf("  255: ");
    print_binary_basic(255);

    printf("\n=== 32-bit Binary Printer ===\n");
    printf("  0:          ");
    print_binary_32(0);
    printf("  1:          ");
    print_binary_32(1);
    printf("  42:         ");
    print_binary_32(42);
    printf("  255:        ");
    print_binary_32(255);
    printf("  256:        ");
    print_binary_32(256);
    printf("  65535:      ");
    print_binary_32(65535);
    printf("  2147483647: ");
    print_binary_32(2147483647);

    printf("\n=== Any-Size Binary Printer (stretch goal) ===\n");
    unsigned char  u8  = 42;
    unsigned short u16 = 1023;
    unsigned int   u32 = 305419896;  /* 0x12345678 */

    printf("  uint8  42:         ");
    print_binary_any_size(&u8, sizeof(u8));
    printf("  uint16 1023:       ");
    print_binary_any_size(&u16, sizeof(u16));
    printf("  uint32 0x12345678: ");
    print_binary_any_size(&u32, sizeof(u32));

    printf("\n=== Bit Facts ===\n");
    printf("  sizeof(char)  = %zu byte(s)  = %zu bits\n",
           sizeof(char), sizeof(char) * CHAR_BIT);
    printf("  sizeof(short) = %zu byte(s)  = %zu bits\n",
           sizeof(short), sizeof(short) * CHAR_BIT);
    printf("  sizeof(int)   = %zu byte(s)  = %zu bits\n",
           sizeof(int), sizeof(int) * CHAR_BIT);
    printf("  sizeof(long)  = %zu byte(s) = %zu bits\n",
           sizeof(long), sizeof(long) * CHAR_BIT);
    printf("  CHAR_BIT      = %d (bits per byte)\n", CHAR_BIT);

    return 0;
}
