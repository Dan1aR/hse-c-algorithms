int cool_strcmp(const char* str1, const char* str2)
{
    unsigned long long i = 0;
    // while someone is not ended
    while ((str1[i] != '\0') && (str2[i] != '\0')) {
        int diff = str1[i] - str2[i];
        if (diff != 0) {
            return diff;
        }
        ++i;
    }
    return str1[i] - str2[i];
}