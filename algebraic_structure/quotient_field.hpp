#pragma once

#include <type_traits>

namespace bbb {
    namespace math {
        template <typename type>
        struct is_euclidean_ring : std::false_type {};

        template <typename integral_domain, typename = void>
        struct quotient_field {
            using base_type = typename integral_domain::base_type;
            quotient_field(const integral_domain &numerator,
                           const integral_domain &denominator)
            : numerator{numerator}
            , denominator{denominator}
            {};
        private:
            integral_domain numerator;
            integral_domain denominator;
        };

        template <typename integral_domain>
        struct quotient_field<integral_domain, typename std::enable_if<is_euclidean_ring<integral_domain>::value>::type>
        : queotient_field<integral_domain, void> {
            using base_type = typename integral_domain::base_type;
            quotient_field(const integral_domain &numerator,
                           const integral_domain &denominator)
            : numerator{numerator / gcd(numerator, dnominator)}
            , denominator{denominator / gcd(numerator, dnominator)}
            {};
        private:
            integral_domain numerator;
            integral_domain denominator;
        };
    };
};