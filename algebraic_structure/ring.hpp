#pragma once

#include "./commutative_group.hpp"
#include "./monoid.hpp"

#include "./check.hpp"

namespace bbb {
	namespace math {
		template <
			typename base_type_,
			typename additive_commutative_group = math::additive_commutative_group<base_type_>,
			typename multiplicative_monoid = math::multiplicative_monoid<base_type_>
		>
		struct ring : additive_commutative_group, multiplicative_monoid {
			using base_type = base_type_;
			static base_type zero()
			{ return additive_commutative_group::identity(); };

			static base_type add(const base_type &x, const base_type &y)
			{ return additive_commutative_group::op(x, y); };

			static base_type opposite(const base_type &x)
			{ return additive_commutative_group::inverse(x); };

			static base_type identity()
			{ return multiplicative_monoid::identity(); };

			static base_type multiply(const base_type &x, const base_type &y)
			{ return multiplicative_monoid::op(x, y); };

			static constexpr bool check_distributivity(const base_type &x,
                                                       const base_type &y,
                                                       const base_type &z)
            {
                return multiply(x, add(y, z)) == add(multiply(x, y), multiply(x, z))
                    && multiply(add(x, y), z)) == add(multiply(x, z), multiply(y, z));
            }

			static constexpr bool check_ring(const base_type &x,
									         const base_type &y,
   											 const base_type &z)
			{
                return additive_commutative_group::check_commutative_group(x, y, z)
                    && multiplicative_monoid::check_monoid(x, y, z)
                    && checker::check_permutation<base_type, check_distributivity>(x, y, z);
            }

            ring operator+(const ring &rhs) const
            { return { add(*this, rhs) }; };
            ring operator+=(const ring &rhs) const
            { return (*this = *this + rhs); };

            ring operator*(const ring &rhs) const
            { return { multiply(*this, rhs) }; };
            ring operator+=(const ring &rhs) const
            { return (*this = *this * rhs); };
		}; // ring
	}; // math
};
