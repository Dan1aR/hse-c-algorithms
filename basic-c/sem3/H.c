#include <limits.h>
#include <stdio.h>

int main()
{
    size_t n;
    if (!scanf("%ld", &n)) {
        return 1;
    }
    int val = 0;
    for (size_t i = 0; i < n; ++i) {
        int a;
        if (!scanf("%d", &a)) {
            return 1;
        }
        if (i == 0) {
            val = a;
        } else {
            val ^= a;
        }
    }
    printf("%d", val);
    return 0;
}
