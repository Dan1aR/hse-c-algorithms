#include <iostream>
#include <random>
#include <unordered_set>
#include <string>
#include <string_view>
#include <span>

#include <sched.h>
#include <pthread.h>

std::size_t bad_find(const std::unordered_set<std::string>& values, std::span<std::string_view> search_values);

std::string GenerateRandomString(std::mt19937& mt)
{
    constexpr std::size_t length = 1000;

    std::string result(length, '\0');
    for (int i = 0; i < length; i++)
        result[i] = '!' + mt() % ('~' - '!');

    return result;
}

int main()
{
    cpu_set_t my_set;        /* Define your cpu_set bit mask. */
    CPU_ZERO(&my_set);       /* Initialize it all to 0, i.e. no CPUs selected. */
    CPU_SET(0, &my_set);     /* set the bit that represents core 7. */
    int rs = sched_setaffinity(0, sizeof(cpu_set_t), &my_set);
    if (rs != 0)
        throw std::runtime_error("Error calling pthread_setaffinity");

    constexpr std::size_t count = 1'000'000;

    std::mt19937 mt(123);

    std::vector<std::string> strings;
    strings.reserve(count);
    for (std::size_t i = 0; i < count; i++)
        strings.emplace_back(GenerateRandomString(mt));

    std::cout << "Generated!" << std::endl;
    getchar();


    std::unordered_set<std::string> index;
    index.reserve(count);
    for (std::size_t i = 0; i < count; i++) {
        if (mt() % 3 == 1)
            index.emplace(strings[i]);
    }
    index.rehash(index.size());

    std::cout << "Filled!" << std::endl;
    getchar();


    std::size_t i = 0;
    std::vector<std::string_view> for_search;
    for_search.reserve(1000);

    while (true) {
        for_search.clear();

        std::size_t for_search_cnt = mt() % 500 + 500;
        if (i + for_search_cnt >= count)
            break;

        for (std::size_t j = 0; j < for_search_cnt; j++)
            for_search.push_back(strings[i + j]);

        std::cout << i << " " << for_search_cnt << " " <<
                     bad_find(index, std::span<std::string_view>{ for_search.begin(), for_search.end() }) << std::endl;
        i += for_search_cnt;
    }

    return 0;
}