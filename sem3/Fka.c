#include <stdio.h>
#include <stdlib.h>
int main()
{
    unsigned int uni = 0;
    unsigned int now = 0;
    char c;
    char b;
    int counter;
    int space;

    while ((c = getchar()) != '\n') {
        unsigned int maska = (1 << (c - 'a'));
        uni |= maska;
    }

    while ((b = getchar()) != '\n') {
        unsigned int maska = (1 << (b - 'a'));
        now |= maska;

        if (b == ' ') {
            if (uni == now) {
                counter++;
            }
            space++;
        }
    }

    printf("%d", counter);
    printf("%d", space);
    return 0;
}