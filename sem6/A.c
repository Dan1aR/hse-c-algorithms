#include <stdio.h>
#include <string.h>
#define MAX_SIZE 1024

int main()
{
    FILE* file;
    if ((file = fopen("lines.txt", "r")) == NULL) {
        return 1;
    }

    char max_str[MAX_SIZE];
    char str[MAX_SIZE];
    size_t max_size = 0;
    while (fgets(str, sizeof(str), file) != NULL) {
        size_t cnt = 0;
        for (size_t i = 0; i < MAX_SIZE; ++i) {
            if ((str[i] == '\n') || (str[i] == '\0')) {
                str[i] = '\n';
                break;
            }
            if (str[i] == ' ') {
                cnt++;
            }
        }
        if (cnt > max_size) {
            max_size = cnt;
            strcpy(max_str, str);
        }
    }
    printf("%s", max_str);
    printf("%ld", max_size + 1);
    return 0;
}
