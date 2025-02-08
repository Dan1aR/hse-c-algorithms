#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 1024
#define ABC_LOWER 97
#define ABC_UPPER 65
#define ABS_SIZE 26

int get_lower_latin_ch(int ch)
{
    if ((ABC_LOWER <= ch) && (ch < ABC_LOWER + ABS_SIZE)) {
        return ch;
    }
    if ((ABC_UPPER <= ch) && (ch < ABC_UPPER + ABS_SIZE)) {
        return (ch - ABC_UPPER) + ABC_LOWER;
    }
    return -1;
}

size_t fill_mutable_buffer(const char* buff, char* const mutable_buffer)
{
    size_t i = 0;
    size_t slow_i = 0;
    while (buff[i] != '\0') {
        int ch = get_lower_latin_ch(buff[i]);
        if (ch != -1) {
            mutable_buffer[slow_i] = ch;
            slow_i++;
        }
        ++i;
    }
    mutable_buffer[slow_i] = '\0';
    return slow_i;
}

int check_palindrom(const char* buff, char* const mutable_buffer)
{
    size_t cleaned_size = fill_mutable_buffer(buff, mutable_buffer);
    for (size_t i = 0; i < cleaned_size / 2; ++i) {
        if (mutable_buffer[i] != mutable_buffer[cleaned_size - i - 1]) {
            return 0;
        }
    }
    return 1;
}

int main()
{
    char buff[MAX_SIZE] = { 0 };
    char mutable_buffer[MAX_SIZE] = { 0 };
    size_t polindroms[100] = { 0 };

    size_t i = 0;
    size_t slow_i = 0;
    while (!feof(stdin)) {
        if (fgets(buff, MAX_SIZE, stdin) == NULL) {
            return -1;
        }
        if (check_palindrom(buff, mutable_buffer) == 1) {
            polindroms[slow_i] = i;
            ++slow_i;
        }
        ++i;
    }
    printf("%ld\n", slow_i);
    for (size_t _slow_i = 0; _slow_i < slow_i; ++_slow_i) {
        printf("%ld ", polindroms[_slow_i]);
    }
    return 0;
}
