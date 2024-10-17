
void* cool_memset(void* str, int c, unsigned long long n)
{
    unsigned char lower_c_byte = (unsigned char)(c & 0xFF);
    // assume str is str and it is
    char* str_ptr = (char*)str;
    for (unsigned long long i = 0; i < n; ++i) {
        str_ptr[i] = lower_c_byte;
    }
    return str;
}
