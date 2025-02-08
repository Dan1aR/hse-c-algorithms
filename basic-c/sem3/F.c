#include <stdio.h>
#define ABC_OFFSET 97

int main()
{
    size_t abc = 0;
    char ch;
    while ((ch = getchar()) != '\n') {
        int code = (int)ch - ABC_OFFSET;
        size_t bit_mask = 1 << code;
        abc |= bit_mask;
    }
    // printf("%ld", abc);

    size_t cnt = 0;
    size_t local_abc = 0;
    while ((ch = getchar()) != '\n') {
        if (ch == ' ') {
            // check stuff;
            if (local_abc == abc) {
                cnt++;
            }
            local_abc = 0;
        } else {
            int code = (int)ch - ABC_OFFSET;
            size_t bit_mask = 1 << code;
            local_abc |= bit_mask;
        }
    }
    if (local_abc == abc) {
        cnt++;
    }
    printf("%ld", cnt);

    return 0;
}