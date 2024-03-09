#include <iostream>
#include <cassert>
#include <chrono>

static std::chrono::time_point<std::chrono::steady_clock> start{};
static std::chrono::time_point<std::chrono::steady_clock> stop{};

void stoper_start() {
    start = std::chrono::steady_clock::now();
}

void stoper_stop() {
    stop = std::chrono::steady_clock::now();
    double elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(stop - start).count();
    std::cout << "Measured time: " << elapsed_seconds << " second(s)\n";
}

// naive sigma function
uint64_t sigma1(uint64_t x) {
    uint64_t acc = 0;
    for (uint64_t i = 1; i <= x; ++i) {
        if (0 == x % i) {
            acc += i;
        }
    }
    return acc;
}

// optimised sigma
// 
uint64_t sigma2(uint64_t x) {
    uint64_t acc = 0;
    for (uint64_t i = 1; i <= x/2; ++i) {
        if (0 == x % i) {
            acc += i;
        }
    }
    return acc + x;
}

int main() {
    assert(12 == sigma1(6));
    assert(12 == sigma2(6));
    
    std::cout << "sigma1\n";
    stoper_start();
    for (uint64_t i = 1; i < 10000; ++i) {
        if (2 * i == sigma1(i)) {
            std::cout << i << '\n';
        }
    }
    stoper_stop();

     std::cout << "sigma2\n";
    stoper_start();
    for (uint64_t i = 1; i < 10000; ++i) {
        if (2 * i == sigma2(i)) {
            std::cout << i << '\n';
        }
    }
    stoper_stop();

    std::cout << "OK!\n";
}
