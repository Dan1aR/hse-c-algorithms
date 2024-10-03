#include <stdio.h>

int main()
{
    size_t n;
    scanf("%lu", &n);
    if (n == 0) {
        printf("1");
        return 0;
    }

    size_t copied_n = n;
    size_t bit_mask = 1;
    while (copied_n) {
        copied_n >>= 1;
        bit_mask <<= 1;
        bit_mask++;
    }
    bit_mask >>= 1;

    size_t full_64bit = 0xFFFFFFFFFFFFFFFF;
    n = (full_64bit - n) & bit_mask;
    printf("%lu", n);

    return 0;
}
