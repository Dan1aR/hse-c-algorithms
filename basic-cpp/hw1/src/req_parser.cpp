#include "req_parser.hpp"

std::optional<Request> StreamingParser::parse_next(std::istream& input)
{
    _reset_state();

    char ch;
    while (input.get(ch)) {
        if (ch == '\n') {
            if (_state == State::READING_VALUE || (_state == State::READING_KEY && !_needs_value)) {
                return _finalize_request();
            } else {
                _state = State::ERROR;
                return std::nullopt;
            }
        }

        if (!_process_character(ch)) {
            return std::nullopt; // ошибка парсинга
        }

        if (_state == State::COMPLETED) {
            return _finalize_request();
        }
    }

    // EOF - завершаем если состояние валидное
    if (_state == State::READING_VALUE || (_state == State::READING_KEY && !_needs_value)) {
        return _finalize_request();
    }

    return std::nullopt;
}
