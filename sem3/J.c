#include <stdio.h>

size_t fact(size_t n)
{
    size_t fact = 1;
    for (size_t i = 1; i <= n; ++i) {
        fact *= i;
    }
    return fact;
}

int main()
{
    // N!/(K!*(N-K)!)
    size_t n, k;
    int __scaned = scanf("%ld %ld", &n, &k);
    if (__scaned < 1) {
        return 1;
    }

    size_t c_n_k = fact(n) / (fact(k) * fact(n - k));
    printf("%ld", c_n_k);

    return 0;
}
