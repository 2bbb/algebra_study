#pragma once

#include <type_traits>

namespace bbb {
	namespace utility {
		template <typename Ty>
		using remove_const_reference = typename std::remove_reference<
			typename std::remove_const<Ty>::type
		>::type;
	}
};
