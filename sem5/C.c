#include <stdio.h>

int cycle_move(int arr[], int new_arr[], size_t offset, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        new_arr[(i + offset) % n] = arr[i];
    }
    return 0;
}

int main()
{
    size_t n;
    size_t offset;
    scanf("%ld", &n);
    scanf("%ld", &offset);

    int arr[100];
    int new_arr[100];
    for (size_t i = 0; i < n; ++i) {
        scanf("%d", &arr[i]);
    }
    cycle_move(arr, new_arr, offset, n);
    for (size_t i = 0; i < n; ++i) {
        printf("%d ", new_arr[i]);
    }

    return 0;
}
