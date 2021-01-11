#pragma once

#include "./additive_group.hpp"

#include <utility>

namespace bbb {
	namespace math {
		namespace ring {
			using namespace additive_group;
			
			template <typename ring>
			ring multiply(const ring &x, const ring &y)
			{ return x * y; };
			
			template <typename ring>
			ring identity()
			{ return {1}; };
		}; // ring
	}; // math
};
