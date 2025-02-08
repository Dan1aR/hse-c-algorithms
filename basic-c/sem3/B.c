#include <stdio.h>

size_t get_dop(size_t n)
{
    if (n == 0) {
        return 1;
    }
    size_t copied_n = n;
    size_t bit_mask = 1;
    while (copied_n > 1) {
        copied_n >>= 1;
        bit_mask <<= 1;
        bit_mask++;
    }
    n = (~n) & bit_mask;
    return n;
}

int main()
{
    size_t n;
    if (!scanf("%lu", &n)) {
        return 1;
    }
    printf("%lu", get_dop(n));

    return 0;
}
