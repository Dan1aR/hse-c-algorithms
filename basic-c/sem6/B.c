#include <stdio.h>
#include <string.h>
#define MAX_SIZE 1024
#define ABC_SIZE 26
#define ABC_OFFSET_SMALL 97
#define ABC_OFFSET_BIG 65

int main()
{
    FILE* file;
    if ((file = fopen("lines.txt", "r")) == NULL) {
        return 1;
    }

    size_t abc[ABC_SIZE] = { 0 };
    char str[MAX_SIZE];
    while (fgets(str, sizeof(str), file) != NULL) {
        for (size_t i = 0; i < MAX_SIZE; ++i) {
            if ((str[i] == '\n') || (str[i] == '\0')) {
                break;
            }
            if ((str[i] >= ABC_OFFSET_BIG) && (str[i] < ABC_OFFSET_BIG + ABC_SIZE)) {
                abc[str[i] - ABC_OFFSET_BIG]++;
            }
            if ((str[i] >= ABC_OFFSET_SMALL) && (str[i] < ABC_OFFSET_SMALL + ABC_SIZE)) {
                abc[str[i] - ABC_OFFSET_SMALL]++;
            }
        }
    }
    for (size_t i = 0; i < ABC_SIZE; ++i) {
        printf("%c: %ld\n", (char)(i + ABC_OFFSET_SMALL), abc[i]);
    }
    return 0;
}
