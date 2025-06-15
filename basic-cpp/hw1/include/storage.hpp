#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "record.hpp"
#include "string_pool.hpp"

class Storage {

private:
    StringPool _string_pool;
    // непосредственно само хранилище id -> data
    std::unordered_map<StringPool::StringId, std::unique_ptr<Record>> _index;

public:
    Storage() = default;
    // нельзя копировать и мувать
    Storage(const Storage& other) = delete;
    Storage& operator=(const Storage& other) = delete;
    Storage(Storage&& other) = delete;
    Storage& operator=(Storage&& other) = delete;

    // сигнатуры set, можно через ссылку на строку, тогда придется скопировать
    void set(const std::string& key, const std::string& value);
    // а можно через rvalue, лучше так
    void set(const std::string& key, std::string&& value);

    // сигнатуры update, можно через ссылку на строку, тогда придется скопировать
    bool update(const std::string& key, const std::string& value);
    // а можно через rvalue, лучше так
    bool update(const std::string& key, std::string&& value);

    // если делаем get, то вернется константная ссылка на Record
    std::optional<std::reference_wrapper<const Record>> get(const std::string& key) const;
    bool del(const std::string& key);
};
