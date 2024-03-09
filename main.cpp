#include <iostream>
#include <cassert>
#include <chrono>
#include <unordered_map>

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

// optimised sigma + LRU
// 
uint64_t sigma3(uint64_t x) {
    static std::unordered_map<uint64_t, uint64_t> m{
        {1, 1},
        {2, 3},
        {3, 4},
        {4, 7},
        {5, 6},
        {6, 12},
        {7, 8},
        {8, 15},
        {9, 13},
        {10, 18},
    };
    if (m.find(x) != m.end()) {
        return m[x];
    } else {
        uint64_t acc = 0;
        for (uint64_t i = 1; i <= x/2; ++i) {
            if (0 == x % i) {
                acc += i;
                if (i != 1 && 0 != (x % (i*i))) {
                    std::cout << "---" << "i=" << i << " x=" << x << " x/i=" << x/i << " acc=" << acc << '\n';
                    m[x] = sigma3(i) * sigma3(x/i);
                    return sigma3(i) * sigma3(x/i);
                }
            }
        }
        acc += x;
        m[x] = acc;
        return acc;
    }
}

int main() {
    assert(12 == sigma1(6));
    assert(12 == sigma2(6));
    std::cout << sigma3(8128) << '\n';
    assert(2*8128 == sigma3(8128));
    
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

    std::cout << "sigma3\n";
    stoper_start();
    for (uint64_t i = 1; i < 10000; ++i) {
        if (2 * i == sigma3(i)) {
            std::cout << i << '\n';
        }
    }
    stoper_stop();

    std::cout << "OK!\n";
}
