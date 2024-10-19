int* cool_int_arr_dup(const int* arr, unsigned long long size, void* (*malloc_func)(unsigned long long num))
{
    int* ptr_int = (int*)malloc_func(sizeof(int) * size);
    for (unsigned long long i = 0; i < size; ++i) {
        ptr_int[i] = arr[i];
    }
    return ptr_int;
}