#pragma once

#include "../magma/multiplicative_magma.hpp"
#include "../semigroup/multiplicative_semigroup.hpp"

namespace bbb {
	namespace math {
		namespace multiplicative_monoid {
			using namespace multiplicative_semigroup;
			
			template <typename monoid>
			monoid identity()
			{ return {1}; };

            template <typename monoid>
            constexpr bool check_identity(const monoid &x) {
                return op(x, identity<monoid>()) == x
                    && op(identity<monoid>(), x) == x;
            };

            template <typename monoid>
            constexpr bool check_monoid(const monoid &x,
                                        const monoid &y,
                                        const monoid &z)
            {
                return check_semigroup(x, y, z)
                    && check_identity(x)
                    && check_identity(y)
                    && check_identity(z);
            }
		};
	};
};
