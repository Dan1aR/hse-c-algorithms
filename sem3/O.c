#include <stdio.h>

size_t bin_to_octal(size_t n)
{
    size_t n_dec = 0;
    while (n) {
        size_t last_bit = n & 1;
        n_dec = n_dec * 8 + last_bit;
        n >>= 1;
    }
    return n_dec;
}

int main()
{
    size_t n;
    if (!scanf("%ld", &n)) {
        return 1;
    }

    for (size_t i = 0; i < n; ++i) {
        size_t level = (1 << (i + 1)) - 1;
        size_t level_dec = bin_to_octal(level);
        printf("%0*lo\n", (int)n, level_dec);
    }

    return 0;
}
