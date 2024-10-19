#include <stdio.h>
#define ROW 9

int check_for_i(unsigned short* field, size_t _i)
{
    unsigned short row = 0;
    unsigned short col = 0;
    unsigned short square = 0;

    size_t square_start_idx = (_i / 3) * 3 * ROW + (_i % 3) * 3;
    for (size_t j = 0; j < ROW; ++j) {
        unsigned short already_set = row & (1 << (field[_i * ROW + j] - 1));
        if (already_set > 0) {
            return 0;
        }
        row |= 1 << (field[_i * ROW + j] - 1);

        already_set = col & (1 << (field[_i + j * ROW] - 1));
        if (already_set > 0) {
            return 0;
        }
        col |= 1 << (field[_i + j * ROW] - 1);

        size_t square_offset = ROW * (j / 3) + j % 3;
        already_set = square & (1 << (field[square_start_idx + square_offset] - 1));
        if (already_set > 0) {
            return 0;
        }
        square |= 1 << (field[square_start_idx + square_offset] - 1);
    }

    return ((row == 511) && (col == 511) && (square == 511)) ? 1 : 0;
}

int check(unsigned short* field)
{
    for (size_t i = 0; i < ROW; ++i) {
        if (check_for_i(field, i) == 0) {
            return 0;
        }
    }
    return 1;
}

int main()
{
    unsigned short field[ROW * ROW] = { 0 };
    for (size_t i = 0; i < ROW; ++i) {
        for (size_t j = 0; j < ROW; ++j) {
            if (scanf("%hu", &field[ROW * i + j]) != 1) {
                return -1;
            }
        }
    }
    printf("%s\n", check(field) ? "valid" : "invalid");

    return 0;
}
