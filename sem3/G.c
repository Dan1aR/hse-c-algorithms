#include <stdio.h>
#define ABC_OFFSET 97
#define DIGIT_OFFSET 48
#define SPACE 32

int main()
{
    char ch;
    // int decimal_offset = 0;
    int cur_num = 0;
    int prev_num = -1;
    size_t correct = 1;
    size_t got_word = 1;
    while ((ch = getchar()) != '\n') {
        if ((int)ch >= ABC_OFFSET) {
            got_word = 1;
            continue;
        }

        if ((int)ch != SPACE) {
            cur_num *= 10;
            cur_num += (int)ch - DIGIT_OFFSET;
            got_word = 0;
        }
        if ((got_word == 0) && ((int)ch == SPACE)) {
            if (cur_num <= prev_num) {
                correct = 0;
                break;
            }
            prev_num = cur_num;
            cur_num = 0;
        }
    }
    if (got_word == 0) {
        if (cur_num <= prev_num) {
            correct = 0;
        }
        prev_num = cur_num;
        cur_num = 0;
    }
    printf("%ld", correct);
    return 0;
}
