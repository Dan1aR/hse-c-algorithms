#include <stdio.h>
#define ABC_SIZE 26
#define ABC_OFFSET 97

int main()
{
    char ch;
    size_t abc[ABC_SIZE] = { 0 };
    while ((ch = getchar()) != '\n') {
        int code = (int)ch - ABC_OFFSET;
        // printf("%c -> %d", ch, code);
        abc[code]++;
    }

    size_t unique_encountered = 0;
    for (size_t i = 0; i < ABC_SIZE; ++i) {
        if (abc[i] == 1) {
            printf("%c", (char)(i + ABC_OFFSET));
            unique_encountered = 1;
        }
    }

    if (unique_encountered == 0) {
        printf("N");
    }

    return 0;
}
