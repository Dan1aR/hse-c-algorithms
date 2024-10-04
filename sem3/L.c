#include <stdio.h>

void print_binary(unsigned long n, size_t bits_max)
{
    for (int i = bits_max - 1; i >= 0; --i) {
        unsigned long mask = 1UL << i;
        printf("%c", (n & mask) ? '}' : '{');
    }
}

size_t check(int bits, int balance, size_t i, size_t bits_max)
{
    // printf("bits=%ld balance=%d i=%ld bits_max=%ld\n", bits, balance, i, bits_max);
    if (i == bits_max) {
        return balance == 0;
    }

    int bits_i = (bits >> (bits_max - i - 1)) & 1;
    // printf("bits_i=%ld\n", bits_i);
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
        // printf("Checking: %ld ", bits);
        // print_binary(bits);
        // printf("\n");
        size_t check_res = check(bits, 0, 0, bits_max);
        // printf("check_res=%ld\n\n", check_res);
        if (check_res == 1) {
            print_binary(bits, bits_max);
            printf("\n");
        }
    }

    return 0;
}
