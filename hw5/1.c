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
    if (fabsf(a - b) < eps) {
        return 0;
    }
    // equal condition is passed now we are
    // definitely > or definitely <
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

int binarySearch(float arr[], size_t low, size_t high, float x, float eps, size_t k, size_t n, size_t* result_index)
{
    *result_index = n;
    while (low <= high) {
        size_t mid = low + (high - low) / 2;
        size_t real_mid = _view_k(mid, k, n);
        int _comp_res = _eps_comp(arr[real_mid], x, eps);
        if (_comp_res == 0) {
            if (real_mid < *result_index) {
                *result_index = real_mid;
            }
            if (mid == 0)
                break;
            high = mid - 1;
        } else if (_comp_res < 0) {
            low = mid + 1;
        } else {
            if (mid == 0)
                break;
            high = mid - 1;
        }
    }
    if (*result_index < n) {
        return 1;
    }
    return 0;
}

int main()
{
    // idea is simple, let's find out what k actually is
    // then we are gonna do bin search, but not with an input
    // array, but with view on in
    // (like in pytorch, no copy, just shift indexes)
    float eps;
    if (scanf("%f", &eps) != 1) {
        return 1;
    }

    size_t N;
    if (scanf("%zu", &N) != 1) {
        return 1;
    }
    float* arr = calloc(N, sizeof(float));
    if (arr == NULL) {
        return 1;
    }
    for (size_t i = 0; i < N; ++i) {
        if (scanf("%f", &arr[i]) != 1) {
            free(arr);
            return 1;
        }
    }
    size_t k = calc_offset(arr, N, eps);
    // Processing with bin search
    size_t M;
    if (scanf("%zu", &M) != 1) {
        free(arr);
        return 1;
    }
    float x = 0;
    for (size_t i = 0; i < M; ++i) {
        if (scanf("%f", &x) != 1) {
            free(arr);
            return 1;
        }
        size_t _res;
        int found = binarySearch(arr, 0, N - 1, x, eps, k, N, &_res);
        if (found) {
            size_t position = _res + 1;
            printf("%zu\n", position);
        } else {
            printf("0\n");
        }
    }
    // O(N + MlogN), O(MlogN) if M ~ N, O(N) if M << N
    return 0;
}
