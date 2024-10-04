#include <stdint.h>
#include <stdio.h>

size_t binary_polindrom_check(uint32_t n)
{
    for (int i = 0; i < 16; ++i) {
        uint32_t mask_right = 1 << i;
        uint32_t mask_left = 1 << (31 - i);
        if (((n & mask_right) >> i) != ((n & mask_left) >> (31 - i))) {
            return 0;
        }
    }
    return 1;
}

int main()
{
    uint32_t n;
    if (!scanf("%d", &n)) {
        return 1;
    }
    printf(binary_polindrom_check(n) ? "true" : "false");

    return 0;
}
