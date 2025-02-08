#include <stdio.h>
#include <string.h>
#define MAX_SIZE 1024
#define ABC_SIZE 26
#define A_LOWER 97
#define A_UPPER 65

int latin_char_detector(const char c)
{
    if (((A_LOWER <= c) && (c < A_LOWER + ABC_SIZE)) || ((A_UPPER <= c) && (c < A_UPPER + ABC_SIZE))) {
        return 1;
    }
    return 0;
}

int main()
{
    FILE* file;
    if ((file = fopen("text.txt", "r")) == NULL) {
        return 1;
    }

    int prev_state = -1;
    size_t cur_num_words = 0;
    size_t max_num_words = 0;
    char max_buff[MAX_SIZE] = { 0 };
    char cur_buff[MAX_SIZE] = { 0 };
    size_t cur_buff_i = 0;

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        // don't count spaces if they are after '.'
        // case 1: "First. Second word." => 1, 2
        // case 2: " First .Second word ." => 1, 2
        // case 3: "    First   .   Second  ,  word   ." => 1, 2
        int latin_char_detected = latin_char_detector(ch);
        // im gonna count word as 0 -> 1 transition
        if (latin_char_detected && ((prev_state == 0) || (prev_state == -1))) {
            ++cur_num_words;
        }
        if (prev_state != -1) {
            cur_buff[cur_buff_i] = ch;
            cur_buff_i++;
        }
        prev_state = latin_char_detected;

        if (ch == '.') {
            // update max size and max buffer;
            if (cur_num_words > max_num_words) {
                strcpy(max_buff, cur_buff);
                max_num_words = cur_num_words;
            }
            // clean up buffer;
            memset(cur_buff, 0, MAX_SIZE);
            cur_buff_i = 0;
            cur_num_words = 0;
            prev_state = -1;
        }
    }
    printf("%s\n", max_buff);
    printf("%ld", max_num_words);

    return 0;
}