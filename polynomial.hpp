#pragma once

#include "./euclidean_ring.hpp"

#include <algorithm>
#include <numeric>
#include <vector>

#include <iostream>

namespace math {
    static const char default_var[] = "x";
    
    template <typename type>
    struct compare0 {
        bool operator()(type v) const
        { return v == type{}; }
    };
    
    template <>
    struct compare0<float> {
        bool operator()(float v) const
        { return v < std::numeric_limits<float>::epsilon(); }
    };
    
    template <>
    struct compare0<double> {
        bool operator()(double v) const
        { return v < std::numeric_limits<float>::epsilon(); }
    };
    
    template <
        typename Ty = float,
        typename Cmp0 = compare0<Ty>,
        const char * const var = default_var
    >
    struct polynomial;
};

namespace math {
    template <typename Ty, typename Cmp0, const char * const var_name>
    std::ostream &operator<<(std::ostream &os, const polynomial<Ty, Cmp0, var_name> &v);
}

namespace util {
    template <typename Ty>
    using remove_const_reference = typename std::remove_reference<
        typename std::remove_const<Ty>::type
    >::type;
}

namespace math {
    template <
        typename RingTy,
        typename Cmp0,
        const char * const var
    >
    struct polynomial {
        using value_type = RingTy;
        
        struct euclid_division_result {
            euclid_division_result() {};
            euclid_division_result(polynomial quotient, polynomial remainder)
            : quotient{std::move(quotient)}
            , remainder{std::move(remainder)}
            {};
            polynomial quotient;
            polynomial remainder;
        };
        
        static polynomial monomial(value_type a, std::size_t degree) {
            std::vector<RingTy> coef;
            coef.resize(degree + 1);
            coef.back() = a;
            return { coef };
        }

        polynomial() {
            coef.resize(1);
            coef[0] = euclidean_ring::zero<value_type>();
        };
        
        polynomial(const polynomial &pr) = default;
        polynomial(polynomial &&pr) = default;
        polynomial &operator=(const polynomial &pr) = default;
        polynomial &operator=(polynomial &&pr) = default;
        
        polynomial(const std::vector<RingTy> &coef)
        : coef{coef}
        {};
        
        polynomial(std::vector<RingTy> &&coef)
        : coef{std::move(coef)}
        {};
        
        template <
            typename X,
            typename ... Xs,
            typename = typename std::enable_if<
                !std::is_same<util::remove_const_reference<X>, polynomial>::value
                && !std::is_same<util::remove_const_reference<X>, std::vector<RingTy>>::value
            >::type
        >
        polynomial(X && v, Xs && ... vs)
        : coef{{value_type{std::forward<X>(v)}, value_type{std::forward<Xs>(vs)} ...}}
        {};
        
        polynomial operator+() const
        { return polynomial{*this}; }
        
        polynomial operator-() const
        { return operator*(euclidean_ring::minus(euclidean_ring::identity<RingTy>())); }
        
#pragma mark +
        polynomial &operator+=(const polynomial &rhs) {
            if(size() < rhs.size()) coef.resize(rhs.size());
            for(auto i = 0; i < rhs.size(); ++i) coef[i] += rhs.at(i);
            return normalize();
        }
        polynomial operator+(const polynomial &rhs) const
        { return polynomial{*this} += rhs; };
        
        template <typename rhs_type>
        polynomial &operator+=(rhs_type scalar)
        { return operator+=(polynomial{scalar}); };
        template <typename lhs_type>
        polynomial operator+(const lhs_type &scalar) const
        { return *this + polynomial{scalar}; }
        
        template <typename lhs_type>
        inline friend polynomial operator+(const lhs_type &scalar, const polynomial &rhs)
        { return polynomial{scalar} + rhs; }

#pragma mark -
        polynomial &operator-=(const polynomial &rhs) {
            while(size() < rhs.size()) coef.resize(rhs.size());
            for(auto i = 0; i < rhs.size(); ++i) {
                coef[i] -= rhs.at(i);
            }
            return normalize();
        }
        polynomial operator-(const polynomial &rhs) const
        { return polynomial{*this} -= rhs; };
        
        template <typename rhs_type>
        polynomial &operator-=(rhs_type scalar)
        { return operator-=(polynomial{scalar}); };
        template <typename lhs_type>
        polynomial operator-(const lhs_type &scalar) const
        { return *this - polynomial{scalar}; }
        
        template <typename lhs_type>
        inline friend polynomial operator-(const lhs_type &scalar, const polynomial &rhs)
        { return polynomial{scalar} - rhs; }

#pragma mark *
        polynomial &operator*=(const polynomial &rhs) {
            decltype(coef) res;
            res.resize(size() + rhs.size() - 1);
            for(auto j = 0; j < size(); ++j) {
                for(auto i = 0; i < rhs.size(); ++i) {
                    res[j + i] += at(j) * rhs.at(i);
                }
            }
            coef = std::move(res);
            return normalize();
        }
        polynomial operator*(const polynomial &rhs) const
        { return polynomial{*this} *= rhs; };
        
        template <typename rhs_type>
        polynomial &operator*=(rhs_type scalar)
        { return operator*=(polynomial{scalar}); };
        template <typename lhs_type>
        polynomial operator*(const lhs_type &scalar) const
        { return *this * polynomial{scalar}; }
        
        template <typename lhs_type>
        inline friend polynomial operator*(const lhs_type &scalar, const polynomial &rhs)
        { return polynomial{scalar} * rhs; };
        
#pragma mark euclid division
    private:
        euclid_division_result euclid_divide_monomial(const polynomial &rhs) const {
            if(degree() < rhs.degree()) {
                return {polynomial{}, *this};
            }
            const auto a = at(degree()) / rhs.at(rhs.degree());
            const auto q = polynomial::monomial(a, degree() - rhs.degree());
            const auto r = operator-(q * rhs);
//            std::cout << *this << ", " << q << ", " << rhs << ", " << r << std::endl;
            return {std::move(q), std::move(r)};
        }
    
    public:    
        euclid_division_result euclid_divide(const polynomial &rhs) const {
            euclid_division_result res{polynomial{}, *this};
            if(rhs.degree() <= degree()) {
                for(std::int64_t i = degree() - rhs.degree(); 0 <= i; i--) {
                    const auto q = std::move(res.quotient);
                    const auto r = std::move(res.remainder);
                    res = r.euclid_divide_monomial(rhs);
                    res.quotient += q;
                }
            }
            return res;
        }
            
#pragma mark /
        polynomial operator/(const polynomial &rhs) const
        { return euclid_divide(rhs).quotient; };
        polynomial &operator/=(const polynomial &rhs) {
            coef = std::move(euclid_divide(rhs).quotient);
            return normalize();
        };
        
        template <typename rhs_type>
        polynomial &operator/=(const rhs_type &scalar) {
            for(auto &c : coef) c /= scalar;
            return normalize();
        }
        
        template <typename rhs_type>
        polynomial operator/(const rhs_type &scalar) const
        { return polynomial{*this} /= scalar; }
        
        inline friend polynomial operator/(value_type scalar, const polynomial &rhs)
        { return polynomial{scalar}.euclid_divide(rhs).quotient; }
        
        polynomial operator%(const polynomial &rhs) const
        { return euclid_divide(rhs).remainder; };
        polynomial &operator%=(const polynomial &rhs) {
            coef = std::move(euclid_divide(rhs).remainder.coef);
            return normalize();
        };

        inline friend polynomial operator%(value_type scalar, const polynomial &rhs)
        { return polynomial{scalar}.euclide_divide(rhs).remainder; }
            
#pragma mark ^
        polynomial operator^(std::uint64_t exponent) {
            auto res = polynomial{1};
            for(auto i = 0; i < exponent; ++i) res *= *this;
            return res;
        }
        polynomial &operator^=(std::uint64_t exponent) {
            auto &&res = operator^(exponent);
            coef = std::move(res.coef);
            return normalize();
        }
        
#pragma mark compare
        bool operator==(const polynomial &rhs) const {
            if(degree() != rhs.degree()) return false;
            for(auto i = 0; i < size(); ++i) if(!Cmp0{}(coef[i] - rhs[i])) return false;
            return true;
        }
        bool operator!=(const polynomial &rhs) const
        { return !(*this == rhs); };
        
        bool operator<(const polynomial &rhs) const {
            if(degree() < rhs.degree()) return true;
            if(rhs.degree() < degree()) return false;
            for(auto i = rhs.degree(); 0 < i; --i) {
                if(at(i) != rhs.at(i)) return at(i) < rhs.at(i);
            }
            return at(0) < rhs.at(0);
        }
        bool operator<=(const polynomial &rhs) const
        { return operator==(rhs) || operator<(rhs); };
        bool operator>(const polynomial &rhs) const
        { return !operator<=(rhs); };
        bool operator>=(const polynomial &rhs) const
        { return !operator<(rhs); };
        
        std::size_t size() const
        { return coef.size(); };
        std::size_t degree() const
        { return size() - 1; };
        
        value_type &leading_coefficient()
        { return coef.back(); };
        value_type leading_coefficient() const
        { return coef.back(); };
        
        value_type &operator[](std::size_t index)
        { return coef.at(index); };
        value_type operator[](std::size_t index) const
        { return coef.at(index); };
        
        value_type &at(std::size_t index)
        { return coef.at(index); };
        value_type at(std::size_t index) const
        { return coef.at(index); };
        
        value_type apply(value_type x) {
            value_type xn = euclidean_ring::identity<value_type>();
            value_type sum = euclidean_ring::zero<value_type>();
            for(auto n = 0; n < size(); ++n) {
                sum += coef[n] * xn;
                xn *= x;
            }
            return sum;
        }
        
    private:
        std::vector<value_type> coef;
        
        polynomial &normalize() {
            while(1 < coef.size() && Cmp0{}(coef.back())) coef.pop_back();
            return *this;
        }
    };
};

#include <iostream>

namespace math {
    template <typename Ty, typename Cmp0, const char * const var_name>
    std::ostream &operator<<(std::ostream &os, const polynomial<Ty, Cmp0, var_name> &v)
    {
        for(auto n = v.size() - 1; 0 < n; n--) {
            if(!Cmp0{}(v[n])) {
                if(v[n] == euclidean_ring::minus(euclidean_ring::identity<Ty>())) os << "-";
                else if(v[n] != euclidean_ring::identity<Ty>()) os << v[n];
                os << var_name << (1 < n ? "^" + std::to_string(n) : "") << " + ";
            }
        }
        return os << v[0];
    }
}
