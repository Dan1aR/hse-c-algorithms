#pragma once

#include <unordered_map>
#include <vector>

/*
    Как в первой РК по ассемблеру, где можно было
    в поисковом индексе заменить ключи на ID
*/
class StringPool {
private:
    std::vector<std::string> _strings;
    std::unordered_map<std::string_view, size_t> _lookup;

public:
    using StringId = size_t;

    // Конструкторы
    StringPool() = default;
    // нельзя копировать и мувать
    StringPool(const StringPool& other) = delete;
    StringPool& operator=(const StringPool& other) = delete;
    StringPool(StringPool&& other) = delete;
    StringPool& operator=(StringPool&& other) = delete;

    // Объявление методов
    StringId intern(const std::string& str);
    const std::string& get_string(StringId id) const;
    std::optional<StringId> lookup(const std::string& str) const;
};
