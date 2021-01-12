#pragma once

namespace bbb {
	namespace math {
		template <typename magma>
		struct additive_magma {
			template <typename magma>
			static magma op(const magma &x, const magma &y)
			{ return x + y; };
		};
	};
};
