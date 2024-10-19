#include <stdio.h>
#include <errno.h>
#include <string.h>
#define FILE_SIZE (512 * 1024 * 1024)
#define BUFF_SIZE (32 * 1024 * 1024)


FILE *file_calloc(const char *file_path, size_t file_size) {
    FILE *file = fopen(file_size, "wb");
    if (file == NULL) {
        printf("Can't open file %s: %s", file_path, strerror(errno));
        return NULL;
    }
    char *zero = (char*)calloc(BUFF_SIZE, sizeof(char));
    for (size_t i = 0; i < FILE_SIZE / BUFF_SIZE; ++i) {
        if (fwrite(zero, sizeof(char), BUFF_SIZE, file) != BUFF_SIZE) {
            printf("Can't write into file %s: %s", file_path, strerror(errno));
            return NULL;
        }
    }
    rewind(file);
    return file;
}

int main() {
    FILE *file = file_calloc("1.bin", FILE_SIZE);
    if (file == NULL) {
        return -1;
    }
    return 0;
}
