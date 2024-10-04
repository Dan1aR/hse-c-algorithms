#include <stdio.h>

void print_binary(size_t n, size_t bits_max)
{
    for (int i = bits_max - 1; i >= 0; --i) {
        size_t mask = 1 << i;
        printf("%c", (n & mask) ? '}' : '{');
    }
}

size_t check(int bits, int balance, size_t i, size_t bits_max)
{
    if (i == bits_max) {
        return balance == 0;
    }

    int bits_i = (bits >> (bits_max - i - 1)) & 1;
    if (bits_i == 0) {
        // open bracket, push on stack
        return check(bits, balance + 1, i + 1, bits_max);
    } else {
        // close bracket, check balance
        if (balance - 1 < 0) {
            // incorrect bracket seq
            return 0;
        }
        return check(bits, balance - 1, i + 1, bits_max);
    }
}

int main()
{
    size_t n;
    int __scaned = scanf("%ld", &n);
    if (__scaned < 1) {
        return 1;
    }

    size_t bits_max = 2 * n;
    size_t num_unique_seq = 1 << bits_max;
    for (int bits = num_unique_seq - 1; bits >= 0; --bits) {
        if (check(bits, 0, 0, bits_max)) {
            print_binary(bits, bits_max);
            printf("\n");
        }
    }

    return 0;
}
