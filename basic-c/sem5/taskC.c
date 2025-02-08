#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARRAY_SZ 1024
#define NEW_LINE_SYMBOL "\n"
#define DELIMETER_SYMBOL " "

typedef size_t sz_t;

void cyclic_shift_right(sz_t n, sz_t m, char** array)
{
    if (m > n) {
        m = n - (m - n) + 1;
    }

    sz_t next_index, current_index = m % (n + 1);
    char* tmp = array[current_index];
    array[current_index] = array[0];

    for (sz_t i = 0; i < n; i++) {
        next_index = (current_index + m) % (n + 1);

        char* to_swap_tmp = array[next_index];
        array[next_index] = tmp;
        tmp = to_swap_tmp;

        current_index = next_index;
    }
}

void print_array(char** array, const size_t sz)
{
    size_t i;
    for (i = 0; i < sz - 1; i++) {
        printf("%s ", array[i]);
    }
    printf("%s \n", array[i]);
}

int main()
{
    sz_t N, M, iterator = 0;
    char tmp_string[MAX_ARRAY_SZ];
    char* array[MAX_ARRAY_SZ];

    if (scanf("%ld %ld", &N, &M) != 2) {
        return -1;
    }
    getchar();
    getchar();

    if (fgets(tmp_string, sizeof(tmp_string), stdin) == NULL) {
        return -1;
    }
    printf("Input: '%s'\n", tmp_string);
    tmp_string[strcspn(tmp_string, NEW_LINE_SYMBOL)] = 0;

    char* token = strtok(tmp_string, DELIMETER_SYMBOL);
    array[iterator++] = token;
    while (token != NULL) {
        token = strtok(NULL, DELIMETER_SYMBOL);
        array[iterator++] = token;
    }

    if (M != 0) {
        if (M == 1) {
            cyclic_shift_right(N - 1, M, array);
        } else {
            cyclic_shift_right(N - 1, M - 1, array);
        }
    }
    print_array(array, N);

    return 0;
}