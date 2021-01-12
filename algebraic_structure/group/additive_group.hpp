#pragma once

#include "../monoid/additive_monoid.hpp"
#include "../check.hpp"

namespace bbb {
	namespace math {
		namespace additive_group {
			using namespace additive_monoid;
			
			template <typename group>
			group inverse(const group &x)
			{ return -x; };

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