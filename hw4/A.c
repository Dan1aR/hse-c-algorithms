#include <stdio.h>

typedef void* (*malloc_func_t)(unsigned long long bytes);
typedef struct {
    char name[64];
    char codec[32];
    size_t size_bytes;
    size_t duration_sec;
    size_t width;
    size_t height;
} video_file_t;
typedef int (*filter_func_t)(const video_file_t*);
typedef int (*is_less_func_t)(const video_file_t*, const video_file_t*);
typedef void (*copy_func_t)(video_file_t* dst, const video_file_t* src);

int partition(video_file_t** vec, int low, int high, const is_less_func_t is_less_func)
{
    video_file_t* pivot = vec[high];
    int i = (low - 1);
    video_file_t* tmp = NULL;

    for (int j = low; j <= high - 1; j++) {
        if (is_less_func(vec[j], pivot) != 0) {
            i++;
            tmp = vec[i];
            vec[i] = vec[j];
            vec[j] = tmp;
        }
    }

    tmp = vec[i + 1];
    vec[i + 1] = vec[high];
    vec[high] = tmp;
    return (i + 1);
}

void quick_sort(video_file_t** vec, int low, int high, const is_less_func_t is_less_func)
{
    if (low < high) {
        int pi = partition(vec, low, high, is_less_func);
        quick_sort(vec, low, pi - 1, is_less_func);
        quick_sort(vec, pi + 1, high, is_less_func);
    }
}

video_file_t** sort_video_files(const video_file_t** files,
    is_less_func_t is_less_func,
    copy_func_t copy_func,
    malloc_func_t malloc_func)
{
    // I can't alter input `files` array, so i first copy, then sort
    size_t _len = 0;
    for (const video_file_t** file_i = files; *file_i != NULL; ++file_i) {
        _len++;
    }
    video_file_t** files_copy = (video_file_t**)malloc_func(sizeof(video_file_t*) * (_len + 1));
    if (files_copy == NULL) {
        return NULL;
    }
    files_copy[_len] = NULL;
    size_t i = 0;
    for (const video_file_t** file_i = files; *file_i != NULL; ++file_i) {
        files_copy[i] = (video_file_t*)malloc_func(sizeof(video_file_t));
        if (files_copy[i] == NULL) {
            return NULL;
        }
        copy_func(files_copy[i], *file_i);
        i++;
    }

    // Copy done, now sort with standard quick sort
    quick_sort(files_copy, 0, _len - 1, is_less_func);
    return files_copy;
}
