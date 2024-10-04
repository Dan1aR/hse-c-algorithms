#include <stdio.h>

int reverse(int arr[], size_t n)
{
    for (size_t i = 0; i < n / 2; ++i) {
        int buff = arr[i];
        arr[i] = arr[n - i - 1];
        arr[n - i - 1] = buff;
    }

    return 0;
}

int main()
{
    size_t n;
    if (!scanf("%ld", &n)) {
        return 1;
    }

    int arr[100];
    for (size_t i = 0; i < n; ++i) {
        if (!scanf("%d", &arr[i])) {
            return 1;
        }
    }
    reverse(arr, n);
    for (size_t i = 0; i < n; ++i) {
        printf("%d ", arr[i]);
    }

    return 0;
}
