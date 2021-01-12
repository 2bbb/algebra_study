#pragma once

#include "./commutative_group/additive_commutative_group.hpp"
#include "./monoid/multiplicative_monoid.hpp"

namespace bbb {
	namespace math {
		namespace ring {
			template <typename ring>
			ring zero()
			{ return additive_commutative_group::identity<ring>(); };

			template <typename ring>
			ring add(const ring &x, const ring &y)
			{ return additive_commutative_group::op<ring>(x, y); };

			template <typename ring>
			ring opposite(const ring &x)
			{ return additive_commutative_group::inverse<ring>(x); };

			template <typename ring>
			ring identity()
			{ return multiplicative_monoid::identity<ring>(); };

			template <typename ring>
			ring multiply(const ring &x, const ring &y)
			{ return multiplicative_monoid::op<ring>(x, y); };

			template <typename ring>
			constexpr bool check_ring(const ring &x,
									  const ring &y,
									  const ring &z)
			{
                return additive_commutative_group::check_commutative_group(x, y, z)
                    && multiplicative_monoid::check_monoid(x, y, z);
            }
		}; // ring
	}; // math
};
