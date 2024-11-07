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
typedef void (*copy_func_t)(video_file_t* dst, const video_file_t* src);

typedef size_t cache_t[1024];
video_file_t** filter_video_files(const video_file_t** files,
    filter_func_t filter_func,
    copy_func_t copy_func,
    malloc_func_t malloc_func)
{
    // idea: let's prefire-filter, than allocate and copy;
    cache_t cache = { 0 };
    size_t slow_i = 0;
    size_t fast_i = 0;
    for (const video_file_t** i = files; *i != NULL; ++i) {
        if (filter_func(*i) != 0) {
            cache[slow_i] = fast_i;
            slow_i++;
        }
        fast_i++;
    }
    if (slow_i == 0) {
        return NULL;
    }

    // now allocate and copy
    video_file_t** copied_files = (video_file_t**)malloc_func(sizeof(video_file_t*) * (slow_i + 1));
    if (copied_files == NULL) {
        return NULL;
    }
    copied_files[slow_i] = NULL;
    for (size_t i = 0; i < slow_i; ++i) {
        copied_files[i] = (video_file_t*)malloc_func(sizeof(video_file_t));
        if (copied_files[i] == NULL) {
            return NULL;
        }
        copy_func(copied_files[i], files[cache[i]]);
    }
    return copied_files;
}
