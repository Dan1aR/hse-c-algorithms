#include <stdio.h>

void* cool_memmove(void* dest, const void* src, unsigned long long bytes)
{
    // src and dest are also strings
    char* src_str = (char*)src;
    char* dest_str = (char*)dest;

    // ok, i can figure out if they are overlappping or not
    int overlapping_offset = 0;

    // printf("src:\n%p\n", src_str);
    // printf("%p\n\n", src_str + bytes);
    // printf("dest:\n%p\n", dest_str);
    // printf("%p\n\n", dest_str + bytes);

    if (((src_str <= dest_str) && (src_str + bytes - 1 >= dest_str)) || ((dest_str <= src_str) && (dest_str + bytes - 1 >= src_str))) {
        overlapping_offset = dest_str - src_str;
    }
    // printf("%d\n", overlapping_offset);

    if (overlapping_offset > 0) {
        for (unsigned long long i = bytes; i >= 1; --i) {
            dest_str[i - 1] = src_str[i - 1];
        }
    } else {
        for (unsigned long long i = 0; i < bytes; ++i) {
            dest_str[i] = src_str[i];
        }
    }

    return dest;
}

int main(void)
{
    // Исходный массив данных
    unsigned char src[5] = "12345";

    // Вывод массива src на консоль
    printf("src old: %s\n", src);

    // Копируем 3 байт
    // unsigned char dest[10] = "0000000000";
    cool_memmove(&src[0], &src[2], 3);

    // Вывод массива src на консоль
    printf("src new: %s\n", src);

    return 0;
}
