#include <limits.h>
#include <stdio.h>

int main()
{
    size_t n;
    if (!scanf("%ld", &n)) {
        return 1;
    }

    int min_price = INT_MAX;
    int max_profit = 0;
    for (size_t i = 0; i < n; ++i) {
        int price;
        if (!scanf("%d", &price)) {
            return 1;
        }
        if (price < min_price) {
            min_price = price;
        } else if ((price - min_price) > max_profit) {
            max_profit = price - min_price;
        }
    }
    printf("%d", max_profit);
}
