/*
 * binary_printer.c — REFERENCE SOLUTION
 *
 * Print any integer in its binary representation.
 */

#include <stdio.h>
#include <limits.h>

void print_binary_basic(unsigned char value)
{
    for (int i = 7; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
    printf("\n");
}

void print_binary_32(unsigned int value)
{
    for (int i = 31; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
        if (i % 4 == 0 && i != 0) {
            printf(" ");
        }
    }
    printf("\n");
}

void print_binary_any_size(const void *data, size_t size)
{
    const unsigned char *bytes = (const unsigned char *)data;

    for (int byte_idx = (int)size - 1; byte_idx >= 0; byte_idx--) {
        for (int bit = 7; bit >= 0; bit--) {
            printf("%d", (bytes[byte_idx] >> bit) & 1);
        }
        if (byte_idx > 0) {
            printf(" ");
        }
    }
    printf("\n");
}


/* ======================================================================
 * TEST HARNESS
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

    printf("\n=== Any-Size Binary Printer ===\n");
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
