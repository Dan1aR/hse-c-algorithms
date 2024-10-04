#include <stdio.h>

int main()
{
    size_t n;
    if (!scanf("%ld", &n)) {
        return 1;
    }
    size_t fact = 1;
    for (size_t i = 1; i <= n; ++i) {
        fact *= i;
    }
    printf("%ld", fact);

    return 0;
}