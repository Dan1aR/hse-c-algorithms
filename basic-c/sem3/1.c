#include <stdio.h>

size_t check(int n)
{
    if (n <= 0) {
        return 0;
    }

    int bin_1_cnt = 0;
    int last_bin_1_pos = 0;
    int i = 0;
    int last_bit = 0;
    while (n > 0) {
        last_bit = n & 1;
        if (last_bit == 1) {
            bin_1_cnt++;
            last_bin_1_pos = i;
        }
        i++;
        n = n >> 1;
    }

    // printf("last_bit = %d", last_bit);
    // printf("bin_1_cnt = %d", bin_1_cnt);
    // printf("last_bin_1_pos = %d", last_bin_1_pos);

    if ((bin_1_cnt > 1) || ((last_bin_1_pos & 1) != 0)) {
        return 0;
    }

    return 1;
}

int main()
{
    int n;
    scanf("%d", &n);
    if (check(n) == 1) {
        printf("true");
    } else {
        printf("false");
    }
    return 0;
}
