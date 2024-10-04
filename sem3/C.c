#include <limits.h>
#include <stdio.h>

int push_max(int* max1, int* max2, int* max3, int a)
{
    /*
        max3 > max2 > max1
    */
    if (a > *max3) {
        int prev_max3 = *max3;
        *max3 = a;
        a = prev_max3;
    }
    if (a > *max2) {
        int prev_max2 = *max2;
        *max2 = a;
        a = prev_max2;
    }
    if (a > *max1) {
        int prev_max1 = *max1;
        *max1 = a;
        a = prev_max1;
    }
    return 0;
}

int push_min(int* min1, int* min2, int* min3, int a)
{
    /*
        min3 < min2 < min1
    */
    if (a < *min3) {
        int prev_min3 = *min3;
        *min3 = a;
        a = prev_min3;
    }
    if (a < *min2) {
        int prev_min2 = *min2;
        *min2 = a;
        a = prev_min2;
    }
    if (a < *min1) {
        int prev_min1 = *min1;
        *min1 = a;
        a = prev_min1;
    }
    return 0;
}

int main()
{
    size_t n;
    if (!scanf("%ld", &n)) {
        return 1;
    }
    int max1 = INT_MIN;
    int max2 = INT_MIN;
    int max3 = INT_MIN;
    int min1 = INT_MAX;
    int min2 = INT_MAX;
    int min3 = INT_MAX;

    for (size_t i = 0; i < n; ++i) {
        int a;
        if (!scanf("%d", &a)) {
            return 1;
        }
        push_max(&max1, &max2, &max3, a);
        push_min(&min1, &min2, &min3, a);
    }

    int max_val = max1 * max2 * max3;
    int min_max_val = min3 * min2 * max3;

    if (max_val > min_max_val) {
        printf("%d", max_val);
    } else {
        printf("%d", min_max_val);
    }

    return 0;
}
