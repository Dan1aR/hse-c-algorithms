#pragma once

#include <string>
#include <utility>

#ifdef DEBUG_RECORD_VERBOSE
#include <iostream>
#define RECORD_DEBUG_VERBOSE(msg) \
    std::cout << "[Record:" << static_cast<const void*>(this) << "] " << msg << std::endl
#else
#define RECORD_DEBUG_VERBOSE(msg)
#endif

class Record {
private:
    std::string _data;

public:
    Record() = default;

    // конструкторы от строки напрямую
    Record(const std::string& str)
        : _data(str)
    {
        RECORD_DEBUG_VERBOSE("Record copy from string constructor");
    }

    // я предполагаю, что я буду читать строку и класть вот сюда rvalue
    Record(std::string&& str)
        : _data(std::move(str))
    {
        RECORD_DEBUG_VERBOSE("Record move from string constructor");
    }

    // Копирование
    Record(const Record& other)
        : _data(other._data)
    {
        RECORD_DEBUG_VERBOSE("Record copy constructor from " << &other);
    }

    Record& operator=(const Record& other)
    {
        if (this == &other) {
            return *this;
        }
        RECORD_DEBUG_VERBOSE("Record copy assignment from " << &other);
        _data = other._data;
        return *this;
    }

    // Move и noexcept для оптимизаций
    Record(Record&& other) noexcept
        : _data(std::move(other._data))
    {
        RECORD_DEBUG_VERBOSE("Record move constructor from " << &other);
    }

    Record& operator=(Record&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }
        RECORD_DEBUG_VERBOSE("Record move assignment from " << &other);
        _data = std::move(other._data);
        return *this;
    }

    ~Record()
    {
        RECORD_DEBUG_VERBOSE("Record destructor");
    }

    const std::string& data() const
    {
        return _data;
    }

    std::string_view view() const
    {
        return _data;
    }
};
