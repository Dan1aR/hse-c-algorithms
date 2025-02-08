#include <stdio.h>

void tail_rec()
{
    char ch = getchar();
    if (ch == '\n') {
        return;
    }
    tail_rec();
    printf("%c", ch);
}

int main()
{
    tail_rec();
    return 0;
}
