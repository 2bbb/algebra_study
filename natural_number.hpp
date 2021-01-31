#pragma once

#include "./utility.hpp"

#include <cstddef>
#include <cstdint>
#include <array>
#include <numeric>
#include <sstream>
#include <iostream>
#include <ios>
#include <iomanip>

namespace bbb {
    template <typename type, std::size_t n>
    struct array {
        using value_type = type;

        template <
            typename ... types,
            typename = typename std::enable_if<sizeof...(types) <= n>::type
        >
        constexpr array(types ... vs)
        : values{static_cast<value_type>(vs) ...}
        {};

        constexpr array(const array &) = default;
        constexpr std::size_t size() const
        { return n; };
        constexpr value_type at(std::size_t index) const
        { return values[index]; }
        constexpr value_type &at(std::size_t index)
        { return values[index]; }

        constexpr value_type operator[](std::size_t index) const
        { return values[index]; }
        constexpr value_type &operator[](std::size_t index)
        { return values[index]; }

        value_type values[n];
    };
};

template <size_t n>
void print(const bbb::array<uint64_t, n> &carry) {
    for(auto i = 0; i < n; ++i) {
        std::cout << carry.at(i) << ", ";
    }
    std::cout << std::endl;
}

namespace bbb {
    namespace math {
        namespace detail {
            constexpr std::size_t max(std::size_t x, std::size_t y)
            { return x < y ? y : x; };
        };

        template <std::size_t digit64, typename = void>
        struct natural_number {
            static constexpr std::size_t max{1'000'000'000ull};
            static constexpr std::size_t carry_digit{9ull};
            // static constexpr std::size_t max{100ull};
            // static constexpr std::size_t carry_digit{2ull};

            using array_t = bbb::array<std::uint64_t, digit64>;

            using iteration = make_index_sequence<digit64>;
            static constexpr iteration is{};

            template <typename ... int_types>
            constexpr natural_number(int_types ... ns)
            : values{normalize({static_cast<std::uint64_t>(ns) ...})}
            , max_digit{calc_max_digit()}
            {};

            constexpr natural_number(const array_t &values)
            : values{normalize(values)}
            , max_digit{calc_max_digit()}
            {};

            constexpr natural_number()
            : values{}
            , max_digit{1}
            {};

            constexpr natural_number(const natural_number &rhs)
            : values{normalize(rhs.values)}
            , max_digit{calc_max_digit()}
            {};

            constexpr natural_number &operator=(const natural_number &rhs) {
                values = normalize(rhs.values);
                max_digit = calc_max_digit();
                return *this;
            };

            static constexpr array_t normalize(const array_t &values)
            { return normalize(values, iteration{}); }
            
            template <std::size_t ... ns>
            static constexpr array_t normalize(const array_t &values_,
                                               index_sequence<ns ...>)
            {
                array_t values = values_;
                bool carry_is_empty = false;
                do {
                    array_t tmp = {
                        ((values.at(ns) % max) + ((ns == 0) ? 0 : (values.at(ns - 1) / max)))
                    ...};
                    values = std::move(tmp);
                    carry_is_empty = true;
                    for(auto i = 0; i < values.size(); ++i)
                    { if(max <= values.at(i)) carry_is_empty = false; };
                } while(!carry_is_empty);

                return values;
            }

            constexpr std::size_t calc_max_digit(std::size_t end = digit64 - 1) const
            {
                while(1 <= end) {
                    if(values[end] != 0) {
                        return end + 1;
                    }
                    end--;
                }
                return 1;
            }

            constexpr natural_number operator+() const
            { return *this; };

            constexpr natural_number operator+(const natural_number &rhs) const
            { return plus(rhs, iteration{}); };

            template <std::size_t ... ns>
            constexpr natural_number plus(const natural_number &rhs,
                                          index_sequence<ns ...>) const 
            {
                return {
                    (values.at(ns) < (max - rhs.values.at(ns))
                        ? (values.at(ns) + rhs.values.at(ns))
                        : (values.at(ns) + (max - rhs.values.at(ns)))
                    ) ...
                };
            }

            static constexpr bool has_carry(const array_t &carry,
                                            std::size_t begin = 0,
                                            std::size_t end = digit64 - 1)
            {
                return (carry.at(begin) != 0) || (carry.at(end) != 0)
                    ? true
                    : begin + 1 == end
                        ? carry.at(begin) != 0
                        : begin + 2 == end
                            ? carry.at(begin + 1) != 0
                            : has_carry(carry, begin + 1, begin + (end - begin) / 2)
                                || has_carry(carry, begin + (end - begin) / 2, end - 1);
            };

            constexpr natural_number &operator+=(const natural_number &rhs)
            { return *this = operator+(rhs); };

            constexpr natural_number operator-(const natural_number &rhs) const
            { return operator<(rhs) ? natural_number{0} : minus(rhs, iteration{}); };

            template <std::size_t ... ns>
            constexpr natural_number minus(const natural_number &rhs,
                                          index_sequence<ns ...>) const
            {
                return  {
                    (
                        values.at(ns) < rhs.values.at(ns)
                            ? values.at(ns) + (max - rhs.values.at(ns))
                            : values.at(ns) - rhs.values.at(ns)
                    ) ...
                };
            }

            constexpr natural_number &operator-=(const natural_number &rhs)
            { return *this = operator-(rhs); };

            constexpr natural_number operator*(const std::uint64_t rhs) const {
                array_t res = values;
                for(auto i = 0; i < max_digit; ++i) {
                    res[i] *= rhs;
                }
                return res;
            };

            constexpr natural_number operator*(const natural_number &rhs) const {
                natural_number res;
                for(auto i = 0; i < rhs.max_digit; ++i) {
                    res += shift(i) * rhs.values.at(i);
                }
                return res;
            };
            
            constexpr natural_number shift(std::size_t n) const
            { return shift_impl(n, iteration{}); };

            template <std::size_t ... ns>
            constexpr natural_number shift_impl(std::size_t n,
                                                index_sequence<ns ...>) const
            { return { (ns < n ? 0 : values.at(ns - n)) ... }; };

            constexpr natural_number &operator*=(const natural_number &rhs)
            { return *this = operator*(rhs); };

            constexpr bool operator==(const natural_number &rhs) const
            { return (max_digit != rhs.max_digit) ? false : eq(rhs, 0); };
            constexpr bool eq(const natural_number &rhs,
                              std::size_t index) const
            {
                return index < max_digit
                    ? values.at(index) == rhs.values.at(index)
                        ? eq(rhs, index + 1)
                        : false
                    : true;
            }    

            constexpr bool operator!=(const natural_number &rhs) const
            { return !operator==(rhs); };

            constexpr bool operator<(const natural_number &rhs) const {
                return max_digit != rhs.max_digit
                    ? max_digit < rhs.max_digit
                    : lt(rhs, max_digit - 1);
            };
            constexpr bool lt(const natural_number &rhs, std::size_t index) const {
                return values.at(index) == rhs.values.at(index)
                    ? 0 < index
                        ? lt(rhs, index - 1)
                        : false
                    : values.at(index) < rhs.values.at(index);
            }
            constexpr bool operator<=(const natural_number &rhs) const
            { return operator==(rhs) || operator<(rhs); };
            constexpr bool operator>(const natural_number &rhs) const
            { return !operator<=(rhs); };
            constexpr bool operator>=(const natural_number &rhs) const
            { return !operator<(rhs); };

            constexpr natural_number frac() const {
                natural_number res = *this;
                for(natural_number i = 2; i < *this; i += 1) {
                    res *= i;
                }
                return res;
            }
            
            array_t values;
            std::size_t max_digit;

            std::string str() const {
                std::ostringstream ss;
                for(std::size_t n = max_digit; 0 < n; --n) {
                    if(n == max_digit) {
                        ss << std::setfill(' ') << values.at(n - 1);
                    } else {
                        ss << std::setfill('0') << std::right << std::setw(carry_digit) << values.at(n - 1);
                    }
                }
                ss << std::setfill(' ');
                return ss.str();
            };
            
            friend std::ostream &operator<<(std::ostream &os, const natural_number &v)
            { return os << v.str(); };
        }; // natural_number<1>
            
        template <>
        struct natural_number<1ull, void> {
            constexpr natural_number(std::uint64_t n)
            : value{n}
            {};

            constexpr natural_number()
            : value{0}
            {};
            
            constexpr natural_number operator+() const
            { return {value}; };

            constexpr natural_number operator+(const natural_number &rhs) const
            { return {value + rhs.value}; };
            natural_number &operator+=(const natural_number &rhs)
            { return *this = operator+(rhs); };

            constexpr natural_number operator-(const natural_number &rhs) const
            { return {rhs.value < value ? value - rhs.value : 0}; };
            natural_number &operator-=(const natural_number &rhs)
            { return *this = operator-(rhs); };

            constexpr natural_number operator*(const natural_number &rhs) const
            { return {value * rhs.value}; };
            natural_number &operator*=(const natural_number &rhs)
            { return *this = operator*(rhs); };

            constexpr bool operator==(const natural_number &rhs) const
            { return value == rhs.value; };
            constexpr bool operator!=(const natural_number &rhs) const
            { return value != rhs.value; };

            constexpr bool operator<(const natural_number &rhs) const
            { return value < rhs.value; };
            constexpr bool operator<=(const natural_number &rhs) const
            { return value <= rhs.value; };
            constexpr bool operator>(const natural_number &rhs) const
            { return value > rhs.value; };
            constexpr bool operator>=(const natural_number &rhs) const
            { return value >= rhs.value; };

            friend std::ostream &operator<<(std::ostream &os, const natural_number &v)
            { return os << v.value; };

            std::uint64_t value;
        }; // natural_number<1>

        namespace literal {
            template <char ... ns>
            constexpr auto operator"" _N()
                -> typename std::enable_if<
                    1 < sizeof...(ns),
                    natural_number<(sizeof...(ns) - 1) / natural_number<2>::carry_digit + 1>
                >::type
            {
                constexpr std::size_t carry_digit = natural_number<2>::carry_digit;
                array<std::uint64_t, (sizeof...(ns) - 1) / carry_digit + 1> res;
                array<char, sizeof...(ns)> cs{ns ...};
                for(auto i = cs.size() - 1; 0 < i; --i) {
                    res[i / carry_digit] = res[i / carry_digit] * 10 + (cs[cs.size() - i - 1] - '0');
                }
                res[0] = res[0] * 10 + cs[cs.size() - 1] - '0';
                return res;
            }
        }; // literal
    }; // math
}; // bbb
