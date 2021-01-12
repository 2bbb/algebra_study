#pragma once

namespace bbb {
	namespace math {
		namespace multiplicative_magma {
			template <typename magma>
			magma op(const magma &x, const magma &y)
			{ return x * y; };
		};
	};
};
