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

}

int main() {

    return 0;
}
