#pragma once

namespace bbb {
	namespace math {
		namespace additive_magma {
			template <typename monoid>
			monoid op(const monoid &x, const monoid &y)
			{ return x + y; };
		};
	};
};
