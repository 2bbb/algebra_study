#pragma once

namespace bbb {
	namespace math {
		namespace additive_group {
			template <typename additive_group>
			additive_group add(const additive_group &x, const additive_group &y)
			{ return x + y; };
			
			template <typename additive_group>
			additive_group zero()
			{ return {0}; };
			
			template <typename additive_group>
			additive_group opposite(const additive_group &x)
			{ return -x; };
		};
	}; // math
};