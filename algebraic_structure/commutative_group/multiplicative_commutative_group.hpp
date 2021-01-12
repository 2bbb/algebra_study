#pragma once

#include "../group/multiplicative_group.hpp"

namespace bbb {
    namespace math {
        namespace multiplicative_commutative_group {
            using namespace multiplicative_group;

            template <typename group>
            constexpr bool check_commutativity(const group &x,
                                               const group &y)
            { return op(x, y) == op(y, x); };

            template <typename commutative_group>
            constexpr bool check_commutative_group(const commutative_group &x,
                                                   const commutative_group &y,
                                                   const commutative_group &z)
            {
                return check_group(x, y, z)
                    && check_commutativity(x, x)
                    && check_commutativity(x, y)
                    && check_commutativity(x, z)
                    && check_commutativity(y, x)
                    && check_commutativity(y, y)
                    && check_commutativity(y, z)
                    && check_commutativity(z, x)
                    && check_commutativity(z, y)
                    && check_commutativity(z, z);
            };
        };
    }
};
