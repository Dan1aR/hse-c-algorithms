#include <stdio.h>

int main()
{
    size_t n;
    int __placeholder = scanf("%ld", &n);
    __placeholder++;

    size_t capacity;
    __placeholder = scanf("%ld", &capacity);
    size_t cur_capacity = capacity;
    size_t num_steps = 0;

    for (size_t i = 0; i < n; ++i) {
        size_t plant;
        __placeholder = scanf("%ld", &plant);

        // printf("plant: %ld\n", plant);
        if (plant <= cur_capacity) {
            cur_capacity -= plant;
            num_steps += 1;
            // printf("1) i: %ld, num_steps: %ld, cur_capacity: %ld\n", i, num_steps, cur_capacity);
        } else {
            // need to go tuda obratno to refill the leiku
            num_steps += i * 2 + 1;
            cur_capacity = capacity - plant;
            // printf("2) i: %ld, num_steps: %ld, cur_capacity: %ld\n", i, num_steps, cur_capacity);
        }
    }
    printf("%ld", num_steps);
    return 0;
}