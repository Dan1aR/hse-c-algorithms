#include <stdio.h>

size_t tail_rec()
{
    char ch = getchar();
    if (ch == '\n') {
        return 0;
    }
    tail_rec();
    printf("%c", ch);
    return 0;
}

int main()
{
    tail_rec();
    return 0;
}
