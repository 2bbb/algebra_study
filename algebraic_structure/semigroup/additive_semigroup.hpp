#pragma once

#include "../magma/additive_magma.hpp"
#include "../check.hpp"

namespace bbb {
    namespace math {
        namespace additive_semigroup {
            using namespace additive_magma;

            template <typename semigroup>
            constexpr bool check_associativity(const semigroup &x,
                                               const semigroup &y,
                                               const semigroup &z)
            {
                return op(op(x, y), z) == op(x, op(y, z));
            };

            template <typename semigroup>
            constexpr bool check_semigroup(const semigroup &x,
                                           const semigroup &y,
                                           const semigroup &z)
            {
                return checker::check_permutation<semigroup, check_associativity>(x, y, z);
            };
        };
    };
};