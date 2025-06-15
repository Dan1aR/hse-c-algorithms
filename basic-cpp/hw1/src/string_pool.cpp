#include "string_pool.hpp"

std::optional<StringPool::StringId> StringPool::lookup(const std::string& str) const
{
    auto it = _lookup.find(str);
    if (it != _lookup.end()) {
        return it->second;
    }
    return std::nullopt;
}

StringPool::StringId StringPool::intern(const std::string& str)
{
    auto it = _lookup.find(str);
    if (it != _lookup.end()) {
        return it->second;
    }

    size_t id = _strings.size();
    _strings.push_back(str);
    _lookup[_strings.back()] = id;
    return id;
}

const std::string& StringPool::get_string(StringPool::StringId id) const
{
    return _strings[id];
}
