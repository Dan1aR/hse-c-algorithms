#include <iostream>
#include <limits>

#include "req_parser.hpp"
#include "storage.hpp"

int main()
{
    Storage storage;
    StreamingParser parser;

    while (true) {
        auto request = parser.parse_next(std::cin);
        if (!request) {
            // защита от мусора в потоке cin
            if (std::cin.eof())
                break;
            std::cout << "Invalid command\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (request->type) {
        case CommandType::SET:
            storage.set(request->key, std::move(*request->value));
            std::cout << "ok" << std::endl;
            break;

        case CommandType::GET: {
            auto record = storage.get(request->key);
            if (!record) {
                std::cout << "not found" << std::endl;
            } else {
                std::cout << record->get().view() << std::endl;
            }
            break;
        }

        case CommandType::UPDATE:
            if (storage.update(request->key, std::move(*request->value))) {
                std::cout << "ok" << std::endl;
            } else {
                std::cout << "not found" << std::endl;
            }
            break;

        case CommandType::DELETE:
            if (storage.del(request->key)) {
                std::cout << "ok" << std::endl;
            } else {
                std::cout << "not found" << std::endl;
            }
            break;

        default:
            std::cout << "Вот этого быть не должно..." << std::endl;
            break;
        }
    }

    return 0;
}