#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILE_SIZE (512 * 1024 * 1024) // 512MB
#define BUFF_SIZE (8 * 1024 * 1024) // 32MB
#define IP_SIZE 16 // 16B
#define ASCII_DIGIT_OFFSET 48

FILE* file_calloc(const char* file_path, size_t file_size)
{
    FILE* file = fopen(file_path, "w+b");
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

size_t get_ip_num(const char* ip)
{
    size_t i = 0;

    size_t octet = 0;
    size_t ip_num = 0;
    while (ip[i] != '\0') {
        if (ip[i] == '.') {
            ip_num <<= 8;
            ip_num |= octet;
            octet = 0;
        } else {
            octet = octet * 10 + (size_t)(ip[i] - ASCII_DIGIT_OFFSET);
        }
        ++i;
    }
    ip_num <<= 8;
    ip_num |= octet;
    return ip_num;
}

int file_ip_write(size_t ip, FILE* file)
{
    size_t byte_index = ip / 8;
    size_t bit_pos = ip % 8;
    if (fseek(file, byte_index, SEEK_SET) != 0) {
        printf("Error seeking in file: %s", strerror(errno));
        fclose(file);
        return -1;
    }
    unsigned char byte_value;
    if (fread(&byte_value, 1, 1, file) != 1) {
        printf("Error reading byte: %s", strerror(errno));
        fclose(file);
        return -1;
    }
    // set ip into byte
    byte_value |= (1 << bit_pos);
    // again seeking back
    if (fseek(file, byte_index, SEEK_SET) != 0) {
        printf("Error seeking in file: %s", strerror(errno));
        fclose(file);
        return -1;
    }
    if (fwrite(&byte_value, 1, 1, file) != 1) {
        printf("Error writing byte into file: %s", strerror(errno));
        fclose(file);
        return -1;
    }
    return 0;
}

int file_ip_read(size_t ip, FILE* file)
{
    size_t byte_index = ip / 8;
    size_t bit_pos = ip % 8;
    if (fseek(file, byte_index, SEEK_SET) != 0) {
        printf("Error seeking in file: %s", strerror(errno));
        fclose(file);
        return -1;
    }
    unsigned char byte_value;
    if (fread(&byte_value, 1, 1, file) != 1) {
        printf("Error reading byte: %s", strerror(errno));
        fclose(file);
        return -1;
    }
    return byte_value & (1 << bit_pos);
}

int main()
{
    // going to preallocate filebuffer 512MB
    FILE* file = file_calloc("/tmp/1.bin", FILE_SIZE);
    if (file == NULL) {
        return -1;
    }

    size_t N;
    if (scanf("%ld", &N) != 1) {
        return -1;
    }

    // allocate char buffer
    char ip_buff[IP_SIZE] = { 0 };
    // write into file
    for (size_t i = 0; i < N; ++i) {
        if (scanf("%s", ip_buff) != 1) {
            return -1;
        }
        size_t ip_num = get_ip_num(ip_buff);
        if (file_ip_write(ip_num, file) != 0) {
            return -1;
        }
    }

    size_t M;
    if (scanf("%ld", &M) != 1) {
        return -1;
    }
    for (size_t i = 0; i < M; ++i) {
        if (scanf("%s", ip_buff) != 1) {
            return -1;
        }
        size_t ip_num = get_ip_num(ip_buff);
        int read_code = file_ip_read(ip_num, file);
        if (read_code == -1) {
            return -1;
        }
        printf("%s\n", (read_code == 0) ? "no" : "yes");
    }
    fclose(file);
    return 0;
}
