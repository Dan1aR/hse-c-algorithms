#include <stdio.h>
#define ABC_SIZE 26
#define ABC_OFFSET 97

int main()
{
    char ch;
    // ok, i can't use array
    // so i can't even do char s[...] to read string
    // or size_t abc[ABC_SIZE] = { 0 };
    // ok, i will implement array in unsigned long long
    // fight the power !!!
    unsigned long long abc = 0;

    while ((ch = getchar()) != '\n') {
        int code = (int)ch - ABC_OFFSET;
        // code is [0..25]
        // abc is 64-bit
        // so i have at least two bits
        // to count chars
        // i guess i don't need more
        // let's get the offset
        int offset = code * 2;
        // getting current value in array
        // move to the right and get last 2 bits via 0b11 mask
        unsigned long long current_val = (abc >> offset) & 0b11;

        // i need to update this val
        // and prevent overflow
        unsigned long long new_val = current_val + 1;
        if (new_val > 3) {
            new_val = 3;
        }

        // ok, now we set this two bits back
        // (2 target bits)______ <- and 0-s from offset
        new_val = new_val << offset;
        abc |= new_val;
        // printf("%c -> %d", ch, code);
    }

    // printf("%lld\n", abc);

    size_t unique_encountered = 0;
    for (size_t i = 0; i < ABC_SIZE; ++i) {
        int offset = i * 2;
        unsigned long long current_val = (abc >> offset) & 0b11;
        if (current_val == 1) {
            printf("%c", (char)(i + ABC_OFFSET));
            unique_encountered = 1;
        }
    }

    if (unique_encountered == 0) {
        printf("N");
    }

    return 0;
}
