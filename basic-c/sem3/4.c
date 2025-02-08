#include <stdio.h>

size_t tail_rec(size_t i, size_t n1, size_t n2, size_t n)
{
    if (i == n) {
        return 0;
    }
    tail_rec(i + 1, n2, n1 + n2, n);
    printf("%ld ", n1);
    return 0;
}

int main()
{
    size_t n;
    scanf("%ld", &n);

    tail_rec(0, 0, 1, n);

    return 0;
}
