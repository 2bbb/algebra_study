#pragma once

#include "./magma.hpp"
#include "./check.hpp"

namespace bbb {
    namespace math {
        template <typename magma>
        struct semigroup : magma {
            using magma::magma;
            static constexpr bool check_associativity(const semigroup &x,
                                                      const semigroup &y,
                                                      const semigroup &z)
            {
                return op(op(x, y), z) == op(x, op(y, z));
            };

            static constexpr bool check_semigroup(const semigroup &x,
                                                  const semigroup &y,
                                                  const semigroup &z)
            {
                return checker::check_permutation<semigroup, check_associativity>(x, y, z);
            };
        };

        template <typename base_type>
        using additive_semigroup = semigroup<additive_magma<base_type>>;
        template <typename base_type>
        using multiplicative_semigroup = semigroup<multiplicative_magma<base_type>>;
    };
};