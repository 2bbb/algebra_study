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
        template <std::size_t digit64>
        struct natural_number {
            using array_t = bbb::array<std::uint64_t, digit64>;
            static constexpr std::size_t max{1000000000000ull};
            static constexpr std::size_t carry_digit{12ull};
            // static constexpr std::size_t max{100ull};
            // static constexpr std::size_t carry_digit{2ull};

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
            : values{{0ul}}
            , max_digit{1}
            {};

            constexpr natural_number(const natural_number &) = default;
            natural_number &operator=(const natural_number &) = default;

            static constexpr array_t normalize(const array_t &values)
            { return normalize(values, iteration{}); }
            
            template <std::size_t ... ns>
            static constexpr array_t normalize(const array_t &values,
                                               index_sequence<ns ...>)
            {
                return normalize_impl(
                    {(values.at(ns) % max) ...},
                    {(values.at(ns) / max) ...},
                    iteration{}
                );
            }
            template <std::size_t ... ns>
            static constexpr array_t normalize_impl(const array_t &values,
                                                    const array_t &carry,
                                                    index_sequence<ns ...>)
            {
                // std::cout << "has_carry: " << has_carry(carry) << std::endl;
                // print(carry);
                return has_carry(carry)
                    ? normalize_impl(
                        {(
                            ns == 0
                            ? values.at(ns)
                            : (values.at(ns) + carry.at(ns - 1)) % max
                        ) ...},
                        {(
                            ns == 0
                            ? 0ul
                            : (values.at(ns) + carry.at(ns - 1)) / max
                        ) ...},
                        iteration{}
                    )
                    : values;
            }

            constexpr std::size_t calc_max_digit(std::size_t begin = 0,
                                                 std::size_t end = digit64 - 1) const
            {
                return begin + 1 == end
                    ? values.at(begin) == 0
                        ? 1ul
                        : end
                    : values.at(end) != 0
                        ? end
                        : 1 < calc_max_digit(begin + (end - begin) / 2, end)
                            ? calc_max_digit(begin + (end - begin) / 2, end)
                            : calc_max_digit(begin, begin + (end - begin) / 2);
            }

            constexpr natural_number operator+() const
            { return *this; };

            constexpr natural_number operator+(const natural_number &rhs) const
            { return plus(rhs, iteration{}); };

            template <std::size_t ... ns>
            constexpr natural_number plus(const natural_number &rhs,
                                          index_sequence<ns ...>) const 
            {
                return plus_impl(
                    {(values.at(ns) < (max - rhs.values.at(ns))
                        ? (values.at(ns) + rhs.values.at(ns))
                        : (values.at(ns) + (max - rhs.values.at(ns)))
                    ) ...},
                    {((values.at(ns) < (max - rhs.values.at(ns))) ? 0ul : 1ul) ... },
                    iteration{}
                );
            }

            template <std::size_t ... ns>
            constexpr natural_number plus_impl(const array_t &result,
                                               const array_t &carry,
                                               index_sequence<ns ...>) const
            {
                return has_carry(carry)
                    ? plus_impl(
                        {(
                            ns == 0
                            ? 0ul
                            : carry.at(ns - 1) == 0ul
                                ? values.at(ns)
                                : (result.at(ns) < (max - carry.at(ns - 1)))
                                    ? (values.at(ns) + carry.at(ns - 1))
                                    : (values.at(ns) + (max - carry.at(ns - 1))
                            )
                        ) ...},
                        {(
                            ns == 0
                            ? 0ul
                            : (result.at(ns) < (max - carry.at(ns - 1)))
                                ? 0ul
                                : 1ul
                        ) ... },
                        iteration{}
                    )
                    : natural_number{result};
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

            natural_number &operator+=(const natural_number &rhs)
            { return *this = operator+(rhs); };

            constexpr natural_number operator-(const natural_number &rhs) const
            { return operator<(rhs) ? natural_number{0} : minus(rhs, iteration{}); };

            template <std::size_t ... ns>
            constexpr natural_number minus(const natural_number &rhs,
                                          index_sequence<ns ...>) const
            {
                return minus_impl(
                    {(
                        values.at(ns) < rhs.values.at(ns)
                            ? values.at(ns) + (max - rhs.values.at(ns))
                            : values.at(ns) - rhs.values.at(ns)
                    ) ...},
                    {(values.at(ns) < rhs.values.at(ns) ? 1ul : 0ul) ...},
                    iteration{}
                );
            }

            template <std::size_t ... ns>
            constexpr natural_number minus_impl(const array_t &result,
                                                const array_t &carry,
                                                index_sequence<ns ...>) const
            {
                return has_carry(carry)
                    ? minus_impl(
                        {(ns == digit64 - 1
                            ? result.at(ns)
                            : (result.at(ns) < carry.at(ns + 1)
                                ? result.at(ns) + (max - carry.at(ns + 1))
                                : result.at(ns) - carry.at(ns + 1)
                            )
                        ) ...},
                        {(ns == digit64 - 1
                            ? 0ul
                            : (result.at(ns) < carry.at(ns + 1)
                                ? 1ul
                                : 0ul
                            )
                        ) ...},
                        iteration{}
                    )
                    : natural_number{result};
            };

            natural_number &operator-=(const natural_number &rhs)
            { return *this = operator-(rhs); };

            constexpr natural_number operator*(const natural_number &rhs) const
            { return mult(rhs); };
            
            constexpr natural_number mult(const natural_number &rhs) const
            { return mult_impl(rhs, iteration{}); };

#if 0
            template <std::size_t ... ns>
            constexpr natural_number mult_impl(const natural_number &rhs,
                                               index_sequence<ns ...>) const
            {
                return {sum(
                    bbb::array<array_t, digit64>{mult_array(ns, rhs.values, iteration{}) ...},
                    iteration{}
                )};
            } 

            template <std::size_t ... ns>
            constexpr array_t mult_array(std::size_t n,
                                         const array_t &vs,
                                         index_sequence<ns ...>) const
            {
                return {(
                    ns < n
                        ? 0
                        : values.at(n) * vs.at(ns - n)
                ) ...}; 
            };


            template <std::size_t ... ns>
            constexpr array_t sum(const bbb::array<array_t, digit64> &vs,
                                  index_sequence<ns ...>) const
            {
                return {sum(vs, ns) ...};
            };

            constexpr std::uint64_t sum(const bbb::array<array_t, digit64> &vs,
                                        std::size_t index,
                                        std::size_t offset = 0ul) const
            {
                return offset == vs.size()
                    ? 0ul
                    : vs.at(offset).at(index) + sum(vs, index, offset + 1);
            };
#endif
#if 1
            template <std::size_t ... ns>
            constexpr natural_number mult_impl(const natural_number &rhs,
                                               index_sequence<ns ...>) const
            {
                return sum({
                    shift_and_scalar(ns, rhs.values, iteration{}) ...
                });
            } 

            template <std::size_t ... ns>
            constexpr natural_number shift_and_scalar(std::size_t n,
                                                      const array_t &vs,
                                                      index_sequence<ns ...>) const
            {
                return {(ns < n
                    ? 0
                    : values.at(n) * vs.at(ns - n)
                ) ...};
            };


            constexpr natural_number sum(const bbb::array<natural_number, digit64> &vs,
                                         std::size_t position = 0,
                                         std::size_t range = digit64 - 1) const
            {
                return position == range
                    ? natural_number{0}
                    : position + 1 == range
                        ? vs.at(position)
                        : sum(vs, position, position + (range - position) / 2)
                        + sum(vs, position + (range - position) / 2, range);
            };
#endif

            natural_number &operator*=(const natural_number &rhs)
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
                return (operator==(0) || operator==(1))
                    ? natural_number{1}
                    : operator*(operator-(1).frac());
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
        };
            
        template <>
        struct natural_number<1ull> {
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
        };
    };
};
