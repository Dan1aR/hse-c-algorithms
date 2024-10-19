#include <stdio.h>
#include <string.h>
#define MAX_SIZE 1024

char* my_strncat(char* destination, const char* append, size_t n)
{
    size_t dest_end = 0;
    for (dest_end = 0; destination[dest_end] != '\0'; ++dest_end)
        ;
    for (size_t i = 0; i < n; ++i) {
        destination[dest_end + i] = append[i];
    }
    return destination;
}

int main()
{
    char str1[2 * MAX_SIZE] = { 0 };
    char str2[MAX_SIZE] = { 0 };
    if (scanf("%s", str1) != 1) {
        return -1;
    }
    if (scanf("%s", str2) != 1) {
        return -1;
    }
    size_t N;
    if (scanf("%ld", &N) != 1) {
        return -1;
    }
    my_strncat(str1, str2, N);
    printf("%s\n", str1);

    return 0;
}
