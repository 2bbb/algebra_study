#pragma once

#include "../monoid/multiplicative_monoid.hpp"

namespace bbb {
	namespace math {
		namespace multiplicative_group {
			using namespace multiplicative_monoid;
			
			template <typename group>
			group inverse(const group &x)
			{ return identity<group>() / x; };

			template <typename group>
			constexpr bool check_inverse(const group &x) {
				return x == identity<group>()
					|| (
						op(x, inverse(x)) == identity<group>()
						&& op(inverse(x), x) == identity<group>()
					);
			}

			template <typename group>
			constexpr bool check_group(const group &x,
				                       const group &y,
									   const group &z)
			{
				return check_semigroup(x, y, z)
					&& check_inverse(x)
					&& check_inverse(y)
					&& check_inverse(z);
			}
		};
	}; // math
};