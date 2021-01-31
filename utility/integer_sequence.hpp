#pragma once

#include <utility>
#include <cstdint>
#include <cstddef>

namespace bbb {
    template <typename int_type, int_type ... ns>
    struct integer_sequence {
        using type = integer_sequence;
        static constexpr std::size_t size = sizeof...(ns);
        constexpr integer_sequence() {};
    };

    template <std::size_t ... ns>
    using index_sequence = integer_sequence<std::size_t, ns ...>;

    namespace detail {
        template<typename seq1, typename seq2> struct concat;

        template<typename int_type, int_type ... xs, int_type ... ys>
        struct concat<integer_sequence<int_type, xs ...>, integer_sequence<int_type, ys ...>>
        : integer_sequence<int_type, xs ..., (sizeof...(xs) + ys) ...> {};

        template <typename int_type, typename n>
        struct make_integer_sequence;

        template <typename int_type, int_type n>
        struct make_integer_sequence<int_type, std::integral_constant<int_type, n>>
        : concat<
            typename make_integer_sequence<int_type, std::integral_constant<int_type, n / 2>>::type,
            typename make_integer_sequence<int_type, std::integral_constant<int_type, n - n /2>>::type
        >::type
        {};

        template <typename int_type>
        struct make_integer_sequence<int_type, std::integral_constant<int_type, 0ull>>
        : integer_sequence<int_type> {};
        
        template <typename int_type>
        struct make_integer_sequence<int_type, std::integral_constant<int_type, 1ull>>
        : integer_sequence<int_type, 0> {};
    };

    template <typename int_type, int_type n>
    using make_integer_sequence = typename detail::make_integer_sequence<int_type, std::integral_constant<int_type, n>>::type;

    template <std::size_t n>
    using make_index_sequence = make_integer_sequence<std::size_t, n>;
};