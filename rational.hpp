#pragma once

#include "./functions.hpp"

#include <numeric>
#include <cmath>

namespace math {
//    template <typename int_type, typename lhs_int, typename rhs_int>
//    constexpr int_type gcd(lhs_int a, rhs_int b)
//    { return a % b ? gcd<int_type>(b, a % b) : b; }
    
    template <typename numerator_t, typename denominator_t>
    constexpr auto irredible_numerator(numerator_t p, denominator_t q)
        -> typename std::enable_if<!std::is_signed<numerator_t>::value, numerator_t>::type
    { return p / gcd(p, q); }
    
    template <typename numerator_t, typename denominator_t>
    constexpr auto irredible_denominator(numerator_t p, denominator_t q)
        -> typename std::enable_if<!std::is_signed<numerator_t>::value, denominator_t>::type
    { return q / gcd(p, q); }

    template <typename numerator_t, typename denominator_t>
    constexpr auto irredible_numerator(numerator_t p, denominator_t q)
        -> typename std::enable_if<std::is_signed<numerator_t>::value, numerator_t>::type
    { return p / gcd(p < 0 ? static_cast<denominator_t>(-p) : static_cast<denominator_t>(p), q); }
    
    template <typename numerator_t, typename denominator_t>
    constexpr auto irredible_denominator(numerator_t p, denominator_t q)
        -> typename std::enable_if<std::is_signed<numerator_t>::value, denominator_t>::type
    { return q / gcd(p < 0 ? static_cast<denominator_t>(-p) : static_cast<denominator_t>(p), q); }
    
    template <typename numerator_t>
    constexpr auto sign(numerator_t p)
        -> typename std::enable_if<!std::is_signed<numerator_t>::value, numerator_t>::type
    { return 1; };
    
    template <typename numerator_t>
    constexpr auto sign(numerator_t p)
    -> typename std::enable_if<std::is_signed<numerator_t>::value, numerator_t>::type
    { return p < 0 ? -1 : 1; };

    template <typename numerator_t, typename denominator_t>
    struct rational {
        constexpr rational(numerator_t p, denominator_t q)
        : p{irredible_numerator(p, q)}
        , q{irredible_denominator(p, q)}
        {};
        
        constexpr rational(numerator_t p)
        : rational{p, 1}
        {};
        
        constexpr rational()
        : rational{0}
        {};
        
        rational(const rational &) = default;
        rational &operator=(const rational &) = default;
                
        constexpr rational operator+() const
        { return {*this}; };
        
        constexpr rational operator-() const
        { return {-p, q}; };
        
        rational &operator+=(const rational &rhs) {
            p = p * rhs.q + rhs.p * q;
            q = q * rhs.q;
            return normalize();
        }
        constexpr rational operator+(const rational &rhs) const
        { return {static_cast<numerator_t>(p * rhs.q) + static_cast<numerator_t>(rhs.p * q), q * rhs.q}; };
        template <typename lhs_type>
        friend constexpr rational operator+(const lhs_type &lhs, const rational &rhs)
        { return rational{lhs} + rhs; };
        
        rational &operator-=(const rational &rhs) {
            p = p * rhs.q - rhs.p * q;
            q = q * rhs.q;
            return normalize();
        }
        constexpr rational operator-(const rational &rhs) const
        { return {static_cast<numerator_t>(p * rhs.q) - static_cast<numerator_t>(rhs.p * q), q * rhs.q}; };
        template <typename lhs_type>
        friend constexpr rational operator-(const lhs_type &lhs, const rational &rhs)
        { return rational{lhs} - rhs; };
        
        rational &operator*=(const rational &rhs) {
            p = p * rhs.p;
            q = q * rhs.q;
            return normalize();
        }
        constexpr rational operator*(const rational &rhs) const
        { return {p * rhs.p, q * rhs.q}; };

//        rational &operator*=(const rational &rhs) {
//            return (*this = operator*(rhs));
//        }
//        constexpr rational operator*(const rational &rhs) const {
//            return {
//                irredible_numerator(p, rhs.q) * irredible_numerator(rhs.p, q),
//                irredible_denominator(rhs.p, q) * irredible_denominator(p, rhs.q)
//            };
//        }
        template <typename lhs_type>
        friend constexpr rational operator*(const lhs_type &lhs, const rational &rhs)
        { return rational{lhs} * rhs; };

        rational &operator/=(const rational &rhs) {
            p = p * (sign(rhs.p) * rhs.q);
            q = q * (sign(rhs.p) * rhs.p);
            return normalize();
        }
        constexpr rational operator/(const rational &rhs) const
        { return {static_cast<numerator_t>(p * (sign(rhs.p) * rhs.q)), q * (sign(rhs.p) * rhs.p)}; };
        template <typename lhs_type>
        friend constexpr rational operator/(const lhs_type &lhs, const rational &rhs)
        { return rational{lhs} / rhs; };
        
        rational operator^(std::uint64_t exponent) {
            auto res = rational{1};
            for(auto i = 0; i < exponent; ++i) res *= *this;
            return res;
        }
        rational &operator^=(std::uint64_t exponent)
        { return (*this = operator^(exponent)); };

        constexpr bool operator==(const rational &rhs) const
        { return p == rhs.p && q == rhs.q; };
        constexpr bool operator!=(const rational &v) const
        { return !operator==(v); };
        
        constexpr bool operator<(const rational &rhs) const
        { return p * rhs.q < rhs.p * q; };
        constexpr bool operator<=(const rational &rhs) const
        { return operator<(rhs) || operator==(rhs); };
        constexpr bool operator>(const rational &rhs) const
        { return !operator<=(rhs); };
        constexpr bool operator>=(const rational &rhs) const
        { return !operator<(rhs); };
        
        constexpr rational normalized() const {
            return p == 0
                ? rational{0, 1}
                : rational{
                    irredible_numerator(p, q),
                    irredible_denominator(p, q)
                };
        }
        
        rational &normalize() {
            if(p == 0) {
                q = 1;
            } else {
                auto &&p_ = std::move(p);
                auto &&q_ = std::move(q);
                p = irredible_numerator(p_, q_);
                q = irredible_denominator(p_, q_);
            }
            return *this;
        }
        
        constexpr numerator_t numerator() const
        { return p; };
        constexpr numerator_t denominator() const
        { return q; };
        
    private:
        numerator_t p{0l};
        denominator_t q{1ul};
    };
};

#include <iostream>

namespace math {
    template <typename numerator_t, typename denominator_t>
    std::ostream &operator<<(std::ostream &os, const rational<numerator_t, denominator_t> &q) {
        if(q.denominator() == 1) {
            return os << q.numerator();
        }
        return os << q.numerator() << " / " << q.denominator();
    }
};
