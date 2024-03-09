#include <iostream>
#include <cassert>
#include <chrono>
#include <unordered_map>
#include <vector>

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

uint64_t power(uint64_t a, uint64_t b) {
    if (0 == b) return 1;
    if (1 == b) return a;
    return a * power(a, b - 1);
}

// factorization but slightly different
// original:
// 34866 = 2 * 3 * 3 * 13 * 149
//       = 2 * 3^2   * 13 * 149 <-- exponentation is used to group together
//                                  multiple copies of the same prime number
// but this funtion returns
// 34866 = 2 *   9   * 13 * 149 <-- so they're no longer prime factors, but
//                                  that's exactly what I need 
//                                  (for sigma function)
std::vector<uint64_t> factorish(uint64_t x) {
    std::vector<uint64_t> result{};
    for (uint64_t i = 2; i <= x/2; ++i) {
        if (0 == x % i) {
            uint64_t foo = 1;
            for (uint64_t j = 1; j < x; ++j) {
                if (0 != (x % power(i,j))) {
                    uint64_t bar = power(i,j-1);
                    result.push_back(bar);
                    auto tail = factorish(x/bar);
                    result.insert(result.end(), tail.begin(), tail.end());
                    return result;
                }
            }
        }
    }
    result.push_back(x);    
    return result;
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
            }
        }
        acc += x;
        m[x] = acc;
        return acc;
    }
}

uint64_t sigma4(uint64_t x) {
    uint64_t result = 1;
    auto fac = factorish(x);
    for (const auto &item : fac) {
        result *= sigma3(item);
    }
    return result;
}

void benchmark() {
    for (const auto &x : factorish(34866)) {
        std::cout << x << ", ";
    }
    std::cout << '\n';
    assert((std::vector<uint64_t>{2,9,13,149} == factorish(34866)));
    assert(12 == sigma1(6));
    assert(12 == sigma2(6));
    std::cout << sigma3(8128) << '\n';
    assert(2*8128 == sigma3(8128));
    
    std::cout << "sigma1\n";
    stoper_start();
    for (uint64_t i = 1; i < 100000; ++i) {
        if (2 * i == sigma1(i)) {
            std::cout << i << '\n';
        }
    }
    stoper_stop();

    std::cout << "sigma2\n";
    stoper_start();
    for (uint64_t i = 1; i < 100000; ++i) {
        if (2 * i == sigma2(i)) {
            std::cout << i << '\n';
        }
    }
    stoper_stop();

    std::cout << "sigma3\n";
    stoper_start();
    for (uint64_t i = 1; i < 100000; ++i) {
        if (2 * i == sigma3(i)) {
            std::cout << i << '\n';
        }
    }
    stoper_stop();

    std::cout << "sigma4\n";
    stoper_start();
    for (uint64_t i = 1; i < 100000; ++i) {
        if (2 * i == sigma4(i)) {
            std::cout << i << '\n';
        }
    }
    stoper_stop();
}

int main() {
    stoper_start();
    auto v = std::vector<uint64_t>{};
    for (uint64_t i = 1; i < 1000000; ++i) {
        if (0 == i % 10000) {
            std::cout << i << '\t';
            for (const auto &item: v) {
                std::cout << item << ", ";
            }
            std::cout << '\n';
        }
        if (2 * i == sigma4(i)) {
            v.push_back(i);
        }
    }
    for (const auto &item: v) {
        std::cout << item << ", ";
    }
    std::cout << '\n';
    stoper_stop();
    std::cout << "OK!\n";
}
