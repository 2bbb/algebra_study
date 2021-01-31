#include <iostream>

#include "./natural_number.hpp"

using namespace bbb::math::literal;

int main(int argc, char *argv[]) {
    constexpr bbb::math::natural_number<45654 / 9 + 1> x{10'000}, y{110};
    // std::cout << "xs:" << xs << std::endl;
    // static_assert(xs.frac() == y, "xs.frac() == y");
    constexpr auto n = 123456789123456789123456789123456789123456789123456789_N;
    std::cout << "123456789123456789123456789123456789123456789123456789_N:\n" << n << std::endl;
    std::cout << "x: " << x << std::endl;
    std::cout << "y: " << y << std::endl;
    std::cout << (x * y) << std::endl;
    std::cout << (x - y) << std::endl;
    // std::cout << x.frac() << std::endl;
}
