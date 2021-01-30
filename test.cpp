#include <iostream>

#include "./natural_number.hpp"

int main(int argc, char *argv[]) {
    constexpr bbb::math::natural_number<256> xs{1000}, y{0};
    // std::cout << "xs:" << xs << std::endl;
    // static_assert(xs.frac() == y, "xs.frac() == y");
    std::cout << xs.frac() << std::endl;
}
