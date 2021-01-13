#pragma once

#include "./group.hpp"
#include "./check.hpp"

namespace bbb {
    namespace math {
        template <typename group>
        struct commutative_group : group {
            using base_type = typename group::base_type;
            using group::group;
            using group::identity;
            using group::inverse;

            template <typename commutative_group>
            static constexpr bool check_commutativity(const commutative_group &x,
                                                      const commutative_group &y)
            { return op(x, y) == op(y, x); };

            template <typename commutative_group>
            static constexpr bool check_commutative_group(const commutative_group &x,
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

        template <typename base_type>
        using additive_commutative_group = commutative_group<additive_group<base_type>>;

        template <typename base_type>
        using multiplicative_commutative_group = commutative_group<multiplicative_group<base_type>>;
    }
};
