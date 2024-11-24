#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int _eps_comp(float a, float b, float eps)
{
    /*
        Compare two floats
        -1 - a < b
        0 - equal
        1 - a > b
    */
    if (fabs(a - b) < eps) {
        return 0;
    }
    // equal condition is passed now we are
    // defenetly > or defenetly <
    return (a < b) ? -1 : 1;
}

size_t calc_offset(float arr[], size_t n, float eps)
{
    size_t k = 0;
    for (size_t i = k + 1; (i < n) && (_eps_comp(arr[k], arr[i], eps) <= 0); ++i) {
        k = i;
    }
    return (k + 1) % n;
}

size_t _view_k(size_t i, size_t k, size_t n)
{
    return (i + k) % n;
}

size_t binarySearch(float arr[], size_t low, size_t high, float x, float eps, size_t k, size_t n)
{
    while (low <= high) {
        size_t mid = low + (high - low) / 2;
        int _comp_res = _eps_comp(arr[_view_k(mid, k, n)], x, eps);
        if (_comp_res == 0)
            return mid;
        if (_comp_res == -1)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

int main()
{

    // idea is simple, let's find out what k actually is
    // then we are gonna do bin search, but not with an input
    // array, but with view on in
    // (like in pytorch, no copy, just shift indexes)
    float eps;
    scanf("%f", &eps);

    size_t N;
    scanf("%ld", &N);
    float* arr = calloc(N, sizeof(float));
    for (size_t i = 0; i < N; ++i) {
        scanf("%f", &arr[i]);
    }
    size_t k = calc_offset(arr, N, eps);
    // Processing with bin search
    size_t M;
    scanf("%ld", &M);
    float x = 0;
    for (size_t i = 0; i < M; ++i) {
        scanf("%f", &x);
        int _res = binarySearch(arr, 0, N - 1, x, eps, k, N);
        if (_res == -1) {
            printf("0\n");
        } else {
            printf("%ld\n", _view_k(_res, k, N) + 1);
        }
    }

    return 0;
}
