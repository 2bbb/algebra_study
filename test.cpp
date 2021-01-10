#include <iostream>

#include "./rational_number.hpp"
#include "./polynomial.hpp"
#include "./quotient_ring.hpp"

struct CF {
    static const math::polynomial<math::rational_number> value() {
        static const math::polynomial<math::rational_number> v{1, 0, 1};
        return v;
    }
};

struct Sq2 {
    static const math::polynomial<math::rational_number> value() {
        static const math::polynomial<math::rational_number> v{-2, 0, 1};
        return v;
    }
};

struct Int3 {
    using type = std::int64_t;
    static constexpr type value() {
        return 3;
    }
};

int main(int argc, char *argv[]) {
    if(false) {
        math::polynomial<math::rational_number> x{1_Q, 2_Q, 3 / 2_Q};
        math::polynomial<math::rational_number> y{1, 1};
        constexpr math::rational_number a{1, 2};
        std::cout << x << std::endl;
        std::cout << (x + y) << std::endl;
        
        auto r = x.euclid_divide(y);
        std::cout << x << " == " << "("  << y << ") * ("<< r.quotient << ") + " << r.remainder << std::endl;
        std::cout << y * r.quotient + r.remainder << std::endl;
    }
    
    if(false) {
        // euclid ring utilities
        math::polynomial<math::rational_number> a{1, 3, 1}, b{1, 1};
        auto &&br = math::euclid_ring::bezout(a, b);
        std::cout << br.x << std::endl;
        std::cout << br.y << std::endl;
        std::cout << br.d << std::endl;
        std::cout << (a * br.x + b * br.y == br.d) << std::endl;
    }

    if(false) {
        math::quotient_ring<Int3> v{1}, w{4};
        std::cout << (2 + v == w + 14) << std::endl;
        std::cout << (v * v == w * w) << std::endl;
    }
    
    if(false) {
        // complex number
        math::quotient_ring<CF> i{0, 1}, one{1}, neg{-1}, zero{0};
        std::cout << (i * i == -1) << std::endl;
        std::cout << ((one + i) * (one - i) == 2) << std::endl;
        std::cout << ((i + one) * (i - one) == -2) << std::endl;
        std::cout << (i + 1) << std::endl;
        std::cout << (2 / (i + 1) == (1 - i)) << std::endl;
    }
    
    if(true) {
        // square_root 2
        math::quotient_ring<Sq2> sq2{0, 1};
        std::cout << (sq2 * sq2 == 2) << std::endl;
        std::cout << (2 / sq2) << ", " << sq2 << std::endl;
        std::cout << (2 / sq2 == sq2) << std::endl;
        std::cout << (1 / (sq2 + 1) == (sq2 - 1)) << std::endl;
    }
}
