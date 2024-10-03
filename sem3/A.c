#include <stdio.h>

int main()
{
    size_t num_bottles, num_exchange;
    scanf("%ld %ld", &num_bottles, &num_exchange);
    size_t total = num_bottles;

    while (num_bottles >= num_exchange) {
        size_t left_after_exchange = num_bottles % num_exchange;
        size_t gained_after_exchange = num_bottles / num_exchange;
        total += gained_after_exchange;
        num_bottles = left_after_exchange + gained_after_exchange;
    }
    printf("%ld", total);

    return 0;
}