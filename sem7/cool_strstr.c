const char* cool_strstr(const char* str1, const char* str2)
{
    // ok, first let's find out their lens
    unsigned long long l_str1 = 0;
    unsigned long long l_str2 = 0;
    while ((str1[l_str1] != '\0') || (str2[l_str2] != '\0')) {
        if (str1[l_str1] != '\0') {
            l_str1++;
        }
        if (str2[l_str2] != '\0') {
            l_str2++;
        }
    }
    if (l_str2 > l_str1) {
        return 0;
    }

    // l_str1 - l_str2 + 1: abcde, ab => 5 - 2 + 1 = 4
    // i = [0, 3] => a, b, c, d
    // even there are two loops the solution is actuall linear
    // because first loop is up to l_str2 - l_str1 + 1
    for (unsigned long long i = 0; i < l_str1 - l_str2 + 1; ++i) {
        short found = 1;
        for (unsigned long long j = 0; j < l_str2; ++j) {
            if (str1[i + j] != str2[j]) {
                found = 0;
                break;
            }
        }
        if (found == 1) {
            return &str1[i];
        }
    }
    return 0;
}