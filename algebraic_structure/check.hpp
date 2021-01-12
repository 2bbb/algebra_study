#pragma once

namespace bbb {
    namespace checker {
        template <typename value_type, bool(checker)(const value_type &, const value_type &)>
        constexpr bool check_permutation(const value_type &x,
                                         const value_type &y)
        {
            return checker(x, x)
                && checker(x, y)
                && checker(y, x)
                && checker(y, y);
        }

        template <typename value_type, bool(checker)(const value_type &, const value_type &, const value_type &)>
        constexpr bool check_permutation_mod(const value_type &x,
                                             const value_type &y,
                                             const value_type &z)
        {
            return checker(x, x, x)
                && checker(x, x, y)
                && checker(x, x, z)
                && checker(x, y, x)
                && checker(x, y, y)
                && checker(x, y, z)
                && checker(x, z, x)
                && checker(x, z, y)
                && checker(x, z, z);
        }

        template <typename value_type, bool(checker)(const value_type &, const value_type &, const value_type &)>
        constexpr bool check_permutation(const value_type &x,
                                         const value_type &y,
                                         const value_type &z)
        {
            return check_permutation_mod<value_type, checker>(x, y, z)
                && check_permutation_mod<value_type, checker>(y, z, x)
                && check_permutation_mod<value_type, checker>(z, x, y);
        }
    };
};