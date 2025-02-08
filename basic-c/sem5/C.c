#include <stdio.h>

int cycle_move(int arr[], int new_arr[], int offset, int n)
{
    for (int i = 0; i < n; ++i) {
        int new_idx = (i - offset) % n;
        while (new_idx < 0) {
            new_idx += n;
        }

        new_arr[new_idx] = arr[i];
    }
    return 0;
}

int main()
{
    int n;
    int offset;
    if (!scanf("%d %d", &n, &offset)) {
        return 1;
    }

    int arr[100];
    int new_arr[100];
    for (int i = 0; i < n; ++i) {
        if (!scanf("%d", &arr[i])) {
            return 1;
        }
    }
    cycle_move(arr, new_arr, offset, n);
    for (int i = 0; i < n; ++i) {
        printf("%d ", new_arr[i]);
    }

    return 0;
}
