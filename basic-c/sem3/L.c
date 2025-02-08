#include <stdio.h>

void print_binary(size_t n, size_t bits_max)
{
    for (int i = bits_max - 1; i >= 0; --i) {
        size_t mask = 1UL << i;
        printf("%c", (n & mask) ? '}' : '{');
    }
}

size_t check(int bits, size_t bits_max)
{
    int balance = 0;
    for (size_t i = 0; i < bits_max; ++i) {
        int bits_i = (bits >> (bits_max - i - 1)) & 1;
        if (bits_i == 0) {
            balance++;
        } else {
            // close bracket, check balance
            if (balance - 1 < 0) {
                // incorrect bracket seq
                return 0;
            }
            balance--;
        }
    }
    return balance == 0;
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
        if (check(bits, bits_max)) {
            print_binary(bits, bits_max);
            printf("\n");
        }
    }

    return 0;
}
