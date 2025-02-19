#include <vector>
#include <algorithm>
#include <cstdint>
#include <unordered_set>
#include <string_view>
#include <string>
#include <span>

// Ищет количество вхождений search_values в values
// И тут мапа и она как бы случайно разложила нам данные
// поэтому в утилите perf будет видно, что page-fault очень большие
std::size_t bad_find(const std::unordered_set<std::string>& values, std::span<std::string_view> search_values)
{
    if (search_values.empty())
        return 0;

    std::vector<std::string> search_values_transformed;
    for (int i = 1; i < search_values.size(); i++) {
        search_values_transformed.push_back(std::string(search_values[i]));
    }
    return values.count(std::string(search_values[0])) + bad_find(values, { search_values.begin() + 1, search_values.end() });
}

/*
    Дело в том как работает malloc, он под собой вызывает системно mmap
    mmap - это просто выделение виртуальной памяти. но физически ниче не выделили
    пока никто из него не читает, никто в него не пишет; То есть mmap он как бы вводит в 
    виртуальную память новые страницы!
    И вот в момент когда происходит первое к ней обращение мы идем и начинаем физические адреса
    притягивать к виртуальным потом что происходит minor page fault;


    И короче у нас получается что в рекурсии хуева гора аллокаций! А аллокация как раз и ведет
    к выделению новой новой памяти за которой надо ходить разрешать физические адреса и получать
    page-fault-ы, которые и прокрасились на perf-e!
*/


