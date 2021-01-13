#pragma once

#include "./semigroup.hpp"

namespace bbb {
	namespace math {
        template <typename semigroup, typename identity_provider>
	    struct monoid : semigroup, identity_provider {
            using base_type = typename semigroup::base_type;
			using semigroup::semigroup;
			using identity_provider::identity;

            static constexpr bool check_identity(const monoid &x) {
                return op(x, identity()) == x
                    && op(identity(), x) == x;
            };

            static constexpr bool check_monoid(const monoid &x,
                                               const monoid &y,
                                               const monoid &z)
            {
                return check_semigroup(x, y, z)
                    && check_identity(x)
                    && check_identity(y)
                    && check_identity(z);
            }
		};

        template <typename semigroup>
        struct additive_identity_provider {
            static typename semigroup::base_type identity() {
                return {0};
            };
        };

        template <typename semigroup>
        struct multiplicative_identity_provider {
            static typename semigroup::base_type identity() {
                return {1};
            };
        };

        template <typename base_type>
        using additive_monoid = monoid<
            additive_semigroup<base_type>,
            additive_identity_provider<additive_semigroup<base_type>>
        >;

        template <typename base_type>
        using multiplicative_monoid = monoid<
            multiplicative_semigroup<base_type>,
            multiplicative_identity_provider<multiplicative_semigroup<base_type>>
        >;
	};
};
