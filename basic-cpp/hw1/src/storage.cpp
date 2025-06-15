#include "storage.hpp"
#include "record.hpp"
#include "string_pool.hpp"
#include <memory>
#include <string>

void Storage::set(const std::string& key, const std::string& value)
{
    auto key_id = _string_pool.intern(key);
    _index[key_id] = std::make_unique<Record>(value);
}

void Storage::set(const std::string& key, std::string&& value)
{
    auto key_id = _string_pool.intern(key);
    _index[key_id] = std::make_unique<Record>(std::move(value));
}

std::optional<std::reference_wrapper<const Record>> Storage::get(const std::string& key) const
{
    auto _key_id = _string_pool.lookup(key);
    if (_key_id == std::nullopt) {
        return std::nullopt;
    }

    StringPool::StringId key_id = *_key_id;
    auto it = _index.find(key_id);
    if (it == _index.end()) {
        return std::nullopt;
    }

    // создаем константную ссылку из unique_ptr<Record>
    return std::cref(*it->second);
}

bool Storage::update(const std::string& key, const std::string& value)
{
    auto _key_id = _string_pool.lookup(key);
    if (_key_id == std::nullopt) {
        return false;
    }

    StringPool::StringId key_id = *_key_id;
    auto it = _index.find(key_id);
    if (it == _index.end()) {
        return false;
    }

    it->second = std::make_unique<Record>(value);
    return true;
}

bool Storage::update(const std::string& key, std::string&& value)
{
    auto _key_id = _string_pool.lookup(key);
    if (_key_id == std::nullopt) {
        return false;
    }

    StringPool::StringId key_id = *_key_id;
    auto it = _index.find(key_id);
    if (it == _index.end()) {
        return false;
    }

    it->second = std::make_unique<Record>(std::move(value));
    return true;
}

bool Storage::del(const std::string& key)
{
    auto _key_id = _string_pool.lookup(key);
    if (_key_id == std::nullopt) {
        return false;
    }

    StringPool::StringId key_id = *_key_id;
    auto it = _index.find(key_id);
    if (it == _index.end()) {
        return false;
    }

    _index.erase(it);

    return true;
}
