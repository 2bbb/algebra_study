#pragma once

#include "./euclidean_ring.hpp"

#include <utility>

namespace bbb {
    namespace math {
        namespace field {
            using namespace euclidean_ring;
            
            template <typename field>
            field inverse(const field &x)
            { return 1 / x; };
            
            template <typename field>
            field divide(const field &x, const field &y)
            { return x / y; };
        };
    }; // math
}; // bbb
