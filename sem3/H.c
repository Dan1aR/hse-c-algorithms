#include <limits.h>
#include <stdio.h>

int main()
{
    size_t n;
    int __placeholder = scanf("%ld", &n);
    __placeholder++;
    int val = 0;
    for (size_t i = 0; i < n; ++i) {
        int a;
        __placeholder = scanf("%d", &a);
        if (i == 0) {
            val = a;
        } else {
            val ^= a;
        }
    }
    printf("%d", val);
    return 0;
}
