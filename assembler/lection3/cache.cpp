#include <vector>
#include <cstdint>
#include <random>
#include <chrono>
#include <iostream>

#include <sched.h>
#include <pthread.h>

struct alignas(128) Num128
{
    union {
        struct {
            uint64_t v1;
            uint64_t v2;
        };

        char bytes[128] = {};
    };
};

int main()
{
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    int rc = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
    if (rc != 0)
        throw std::runtime_error("Error calling pthread_setaffinity");

    //uint32_t sz = 20*1024;
    //size_t sz = 1*1024*1024*1024;
    //size_t end_sz = 1*1024*1024*1024;
    size_t sz = 1*1024;
    size_t end_sz = 1*1024*1024*1024;
    uint32_t step = 1024;

    constexpr uint32_t Iterations = 100000;
    for (; sz <= end_sz; sz *= 2) {
        std::mt19937_64 mt(123);

        std::vector<Num128> nums(sz / sizeof(Num128));
        for (uint32_t i = 0; i < nums.size(); i++) {
            nums[i].v1 = mt();
            nums[i].v2 = mt();
        }

        //std::vector<uint32_t> order(Iterations);
        //for (uint32_t i = 0; i < Iterations; i++)
        //    order[i] = mt() % sz;


        // warmup
        //volatile double result = 0.;
        volatile uint64_t pos = 0;
        volatile uint64_t result = 0;
        for (uint32_t i = 0; i < nums.size(); i++) {
            result += nums[pos].v2;
        }

        // measure
        auto start = std::chrono::high_resolution_clock::now();
        for (uint32_t i = 0; i < Iterations; i++) {
            //result = result + nums[pos].v2;
            pos = nums[i % nums.size()].v2 % nums.size();
            for (uint32_t i = 0; i < 100; i++) {
                pos += (nums[pos].v1) % 100 + 1;
                pos = (pos % nums.size());
            }
        }
        auto finish = std::chrono::high_resolution_clock::now();
        std::cout << sz / 1024 << "\t" << std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() / 1000 << "\tmu_s, pos= " << pos << "\n";

    }
}

