#pragma once

#include "./monoid.hpp"
#include "./check.hpp"

namespace bbb {
	namespace math {
        template <
            typename monoid,
            typename inverse_provider
        >
		struct group : monoid, inverse_provider {
            using base_type = typename monoid::base_type;
			using monoid::monoid;
            using monoid::identity;
            using inverse_provider::inverse;

			static constexpr bool check_inverse(const group &x) {
				return x() == identity()
					|| (
						op(x, inverse(x)) == identity()
						&& op(inverse(x), x) == identity()
					);
			}

			static constexpr bool check_group(const group &x,
                                              const group &y,
                                              const group &z)
			{
				return monoid::check_monoid(x, y, z)
					&& check_inverse(x)
					&& check_inverse(y)
					&& check_inverse(z);
			}
		};

        template <typename monoid>
        struct additive_inverse_provider {
            static typename monoid::base_type inverse(const monoid &x) {
                return -(x());
            };
        };

        template <typename monoid>
        struct multiplicative_inverse_provider {
            static typename monoid::base_type inverse(const monoid &x) {
                return monoid::identity() / x();
            };
        };

        template <typename base_type>
        using additive_group = group<
            additive_monoid<base_type>,
            additive_inverse_provider<additive_monoid<base_type>>
        >;

        template <typename base_type>
        using multiplicative_group = group<
            multiplicative_monoid<base_type>,
            multiplicative_inverse_provider<multiplicative_monoid<base_type>>
        >;

	}; // math
};