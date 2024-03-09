#include <iostream>
#include <cassert>

uint64_t sigma(uint64_t x) {
    uint64_t acc = 0;
    for (uint64_t i = 1; i <= x; ++i) {
        if (0 == x % i) {
            acc += i;
        }
    }
    return acc;
}

int main() {
    assert(12 == sigma(6));
    for (uint64_t i = 5; i < 8200; ++i) {
        if (2 * i == sigma(i)) {
            std::cout << i << '\n';
        }
    }
    std::cout << "OK!\n";
}
