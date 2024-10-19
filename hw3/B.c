#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILE_SIZE (512 * 1024 * 1024)
#define BUFF_SIZE (8 * 1024 * 1024)

FILE* file_calloc(const char* file_path, size_t file_size)
{
    FILE* file = fopen(file_path, "wb");
    if (file == NULL) {
        printf("Can't open file %s: %s", file_path, strerror(errno));
        return NULL;
    }

    char* zero = (char*)calloc(BUFF_SIZE, sizeof(char));
    if (zero == NULL) {
        printf("Buffer calloc failed: %s", strerror(errno));
        fclose(file);
        return NULL;
    }
    size_t full_chunks = file_size / BUFF_SIZE;
    size_t remaining_bytes = file_size % BUFF_SIZE;
    for (size_t i = 0; i < full_chunks; ++i) {
        if (fwrite(zero, sizeof(char), BUFF_SIZE, file) != BUFF_SIZE) {
            printf("Can't write into file %s: %s", file_path, strerror(errno));
            free(zero);
            fclose(file);
            return NULL;
        }
    }
    if (remaining_bytes > 0) {
        if (fwrite(zero, sizeof(char), remaining_bytes, file) != remaining_bytes) {
            printf("Can't write remaining bytes into file %s: %s\n", file_path, strerror(errno));
            free(zero);
            fclose(file);
            return NULL;
        }
    }
    free(zero);
    rewind(file);
    return file;
}

int main()
{
    FILE* file = file_calloc("1.bin", FILE_SIZE);
    if (file == NULL) {
        return -1;
    }
    return 0;
}
