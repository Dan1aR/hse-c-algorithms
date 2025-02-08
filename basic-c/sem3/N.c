#include <stdio.h>

void print_level(size_t n, size_t bits_max)
{
    for (int i = bits_max - 1; i >= 0; --i) {
        size_t mask = 1UL << i;
        printf("%c", (n & mask) ? '*' : '.');
    }
    printf("\n");
}

int main()
{
    size_t n;
    if (!scanf("%ld", &n)) {
        return 1;
    }
    size_t level_size = 2 * n - 1;
    size_t level_mask_num = 2;

    for (size_t i = 0; i < n; ++i) {
        size_t level_mask = (level_mask_num - 1) << (n - i - 1);
        print_level(level_mask, level_size);
        level_mask_num <<= 2;
    }

    return 0;
}
