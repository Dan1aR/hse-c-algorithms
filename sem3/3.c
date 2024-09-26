#include <stdio.h>

int main()
{
    unsigned long long a;
    scanf("%lld", &a);
    printf("0x%llx\n", a);
    // count number of
    // znachashix bits
    size_t mask = 0b1111;
    size_t cnt = 0;
    while (a) {
        if (a & mask) {
            cnt++;
        }
        a >>= 4;
    }
    printf("%ld", cnt + 2);

    return 0;
}
