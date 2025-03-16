#pragma once

#include <ctime>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <unordered_set>


constexpr std::string_view IndexName = "index.bin";

class Timer
{
public:
    explicit Timer(std::string prefix) :
        prefix_(std::move(prefix)),
        start_(std::clock())
    {}

    template<typename F>
    static double ExecuteWithTime(const F& func)
    {
        Timer t("");
        func();
        return t.Now();
    }

    static void Output(std::string_view prefix, double elapsed)
    {
        std::cerr << "#!TIMER\t" << prefix<< "\t" << elapsed << std::endl;
    }

    double Now() const
    {
        std::clock_t now = std::clock();
        return 1000. * (now - start_) / CLOCKS_PER_SEC;
    }

    void Report() const
    {
        double elapsed = Now();
        Output(prefix_, elapsed);
    }

private:
    std::string prefix_;
    std::clock_t start_;
};

std::vector<std::string> split(std::string inp, char c)
{
    std::vector<std::string> result;
    while (true) {
        if (inp.empty())
            break;

        auto pos = inp.find_first_of(c);
        if (pos != std::string::npos) {
            if (pos != 0) // Empty substring
                result.push_back(inp.substr(0, pos));
            inp = inp.substr(pos + 1);
        } else {
            result.push_back(inp);
            break;
        }
    }
    return result;
}

