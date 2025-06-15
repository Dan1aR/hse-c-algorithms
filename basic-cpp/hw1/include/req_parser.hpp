#pragma once
#include <iostream>
#include <optional>
#include <string>

enum class CommandType {
    SET,
    GET,
    UPDATE,
    DELETE
};

struct Request {
    CommandType type;
    std::string key;
    std::optional<std::string> value;
};

class StreamingParser {
private:
    enum class State {
        READING_COMMAND,
        READING_KEY,
        READING_VALUE,
        COMPLETED,
        ERROR
    };

    State _state = State::READING_COMMAND;
    std::string _command_buffer;
    std::string _key_buffer;
    std::string _value_buffer;

    CommandType _current_command_type;
    bool _needs_value = false;

public:
    std::optional<Request> parse_next(std::istream& input);

private:
    void _reset_state()
    {
        _state = State::READING_COMMAND;
        _command_buffer.clear();
        _key_buffer.clear();
        _value_buffer.clear();
        _needs_value = false;
    }

    bool _process_character(char ch)
    {
        switch (_state) {
        case State::READING_COMMAND:
            return _process_command_char(ch);
        case State::READING_KEY:
            return _process_key_char(ch);
        case State::READING_VALUE:
            return _process_value_char(ch);
        case State::ERROR:
            return false;
        case State::COMPLETED:
            return true;
        }
        return false;
    }

    bool _process_command_char(char ch)
    {
        if (ch == ' ') {
            // Команда завершена, определяем тип
            if (!_parse_command_type()) {
                _state = State::ERROR;
                return false;
            }
            _state = State::READING_KEY;
            return true;
        }

        if (std::isalpha(ch)) {
            _command_buffer += std::toupper(ch); // команды в верхнем регистре
            return true;
        }

        _state = State::ERROR;
        return false;
    }

    bool _process_key_char(char ch)
    {
        if (ch == ' ') {
            if (_key_buffer.empty()) {
                _state = State::ERROR;
                return false;
            }

            if (_needs_value) {
                _state = State::READING_VALUE;
            } else {
                _state = State::COMPLETED;
            }
            return true;
        }

        // Ключ не может содержать пробелы (по нашим правилам)
        if (ch != ' ' && ch != '\t' && ch != '\n') {
            _key_buffer += ch;
            return true;
        }

        _state = State::ERROR;
        return false;
    }

    bool _process_value_char(char ch)
    {
        // Значение может содержать любые символы кроме \n
        _value_buffer += ch;
        return true;
    }

    bool _parse_command_type()
    {
        if (_command_buffer == "SET") {
            _current_command_type = CommandType::SET;
            _needs_value = true;
        } else if (_command_buffer == "GET") {
            _current_command_type = CommandType::GET;
            _needs_value = false;
        } else if (_command_buffer == "UPDATE") {
            _current_command_type = CommandType::UPDATE;
            _needs_value = true;
        } else if (_command_buffer == "DELETE") {
            _current_command_type = CommandType::DELETE;
            _needs_value = false;
        } else {
            return false;
        }
        return true;
    }

    Request _finalize_request()
    {
        _state = State::COMPLETED;

        if (_needs_value) {
            return Request {
                _current_command_type,
                std::move(_key_buffer),
                std::move(_value_buffer)
            };
        } else {
            return Request {
                _current_command_type,
                std::move(_key_buffer),
                std::nullopt
            };
        }
    }
};
