#include <iostream>

#include "./natural_number.hpp"

using namespace bbb::math::literal;

namespace bbb {
    namespace math {
        void test_natural_number() {
            {
                constexpr auto x = 1'111'111'111'111'111_N;
                constexpr auto y = 2'222'222'222'222'222_N;
                
                std::cout <<            x            << std::endl;
                std::cout <<            y            << std::endl;
                std::cout <<         (x * x)         << std::endl;
                std::cout << 3'333'333'333'333'333_N << std::endl;
                
                static_assert(x + y == 3'333'333'333'333'333_N,
                             "x + y == 3'333'333'333'333'333_N");
            };
        };
    };
};

int main(int argc, char *argv[]) {
    bbb::math::test_natural_number();
}
