#pragma once

#include "./functions.hpp"
#include "./rational.hpp"

#include <numeric>
#include <cmath>

namespace bbb {
    namespace math {
        using rational_number = rational<std::int64_t, std::uint64_t>;
        //    struct rational_number {
        //        constexpr rational_number(std::int64_t p, std::uint64_t q)
        //        : p{p / gcd<std::int64_t>(p < 0ll ? -p : p, q)}
        //        , q{q / gcd<std::uint64_t>(p < 0ll ? -p : p, q)}
        //        {};
        //        
        //        constexpr rational_number(std::int64_t p)
        //        : rational_number{p, 1ull}
        //        {};
        //        
        //        constexpr rational_number()
        //        : rational_number{0ll}
        //        {};
        //        
        //        rational_number(const rational_number &) = default;
        //        rational_number &operator=(const rational_number &) = default;
        //                
        //        constexpr rational_number operator+() const
        //        { return {*this}; };
        //        
        //        constexpr rational_number operator-() const
        //        { return {-p, q}; };
        //        
        //        rational_number &operator+=(const rational_number &rhs) {
        //            p = p * rhs.q + rhs.p * q;
        //            q = q * rhs.q;
        //            return normalize();
        //        }
        //        constexpr rational_number operator+(const rational_number &rhs) const
        //        { return {static_cast<std::int64_t>(p * rhs.q) + static_cast<std::int64_t>(rhs.p * q), q * rhs.q}; };
        //        template <typename lhs_type>
        //        friend constexpr rational_number operator+(const lhs_type &lhs, const rational_number &rhs)
        //        { return rational_number{lhs} + rhs; };
        //        
        //        rational_number &operator-=(const rational_number &rhs) {
        //            p = p * rhs.q - rhs.p * q;
        //            q = q * rhs.q;
        //            return normalize();
        //        }
        //        constexpr rational_number operator-(const rational_number &rhs) const
        //        { return {static_cast<std::int64_t>(p * rhs.q) - static_cast<std::int64_t>(rhs.p * q), q * rhs.q}; };
        //        template <typename lhs_type>
        //        friend constexpr rational_number operator-(const lhs_type &lhs, const rational_number &rhs)
        //        { return rational_number{lhs} - rhs; };
        //        
        //        rational_number &operator*=(const rational_number &rhs) {
        //            p = p * rhs.p;
        //            q = q * rhs.q;
        //            return normalize();
        //        }
        //        constexpr rational_number operator*(const rational_number &rhs) const
        //        { return {p * rhs.p, q * rhs.q}; };
        //        template <typename lhs_type>
        //        friend constexpr rational_number operator*(const lhs_type &lhs, const rational_number &rhs)
        //        { return rational_number{lhs} * rhs; };
        //
        //        rational_number &operator/=(const rational_number &rhs) {
        //            p = p * rhs.q;
        //            q = q * rhs.p;
        //            return normalize();
        //        }
        //        constexpr rational_number operator/(const rational_number &rhs) const
        //        { return {static_cast<std::int64_t>(p * rhs.q), q * rhs.p}; };
        //        template <typename lhs_type>
        //        friend constexpr rational_number operator/(const lhs_type &lhs, const rational_number &rhs)
        //        { return rational_number{lhs} / rhs; };
        //        
        //        constexpr bool operator==(const rational_number &rhs) const
        //        { return p == rhs.p && q == rhs.q; };
        //        constexpr bool operator!=(const rational_number &v) const
        //        { return !operator==(v); };
        //        
        //        constexpr bool operator<(const rational_number &rhs) const
        //        { return p * rhs.q < rhs.p * q; };
        //        constexpr bool operator<=(const rational_number &rhs) const
        //        { return operator<(rhs) || operator==(rhs); };
        //        constexpr bool operator>(const rational_number &rhs) const
        //        { return !operator<=(rhs); };
        //        constexpr bool operator>=(const rational_number &rhs) const
        //        { return !operator<(rhs); };
        //        
        //        constexpr rational_number normalized() const {
        //            return p == 0 ? rational_number{0, 1}
        //            : rational_number{
        //                p / gcd<std::int64_t>(p < 0 ? -p : p, q),
        //                q / gcd<std::uint64_t>(p < 0 ? -p : p , q)
        //            };
        //        }
        //        
        //        rational_number &normalize() {
        //            if(p == 0) {
        //                q = 1;
        //            } else {
        //                const auto g = gcd<std::uint64_t>(p < 0 ? -p : p, q);
        //                p /= g;
        //                q /= g;
        //            }
        //            return *this;
        //        }
        //        
        //        constexpr std::int64_t numerator() const
        //        { return p; };
        //        constexpr std::int64_t denominator() const
        //        { return q; };
        //        
        //    private:
        //        std::int64_t p{0l};
        //        std::uint64_t q{1ul};
        //    };
        
        template <std::int64_t p, std::uint64_t q>
        struct rational_number_t {
            static constexpr rational_number value{p, q};
        };
        
        namespace rational_number_user_defined_literal {
            rational_number operator""_Q(std::uint64_t v)
            { return {static_cast<std::int64_t>(v), 1}; };
        };
    };
    
    using namespace math::rational_number_user_defined_literal;
};

#include <iostream>

namespace bbb {    
    namespace math {
        std::ostream &operator<<(std::ostream &os, const rational_number &q) {
            if(q.denominator() == 1) {
                return os << q.numerator();
            }
            return os << q.numerator() << " / " << q.denominator();
        }
    };
};
