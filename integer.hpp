#pragma once

namespace bbb {
    namespace math {
        template <typename natural_number>
        struct integer {
            constexpr integer(natural_number positive, natural_number negative)
            : positive{positive}
            , negative{negative}
            {};
            constexpr integer(natural_number positive)
            : integer{positive, 0}
            {};
            constexpr integer()
            : integer{0}
            {};
            
            constexpr integer operator+() const
            { return {positive, negative}; };
            constexpr integer operator-() const
            { return {negative, positive}; };

            constexpr integer operator+(const integer &rhs) const 
            { return {positive + rhs.positive, negative + rhs.negative}; };
            integer &operator+=(const integer &rhs)
            { return *this = operator+(*this); };

            constexpr integer operator-(const integer &rhs) const 
            { return {positive + rhs.negative, negative + rhs.positive}; };
            integer &operator-=(const integer &rhs)
            { return *this = operator-(*this); };

            constexpr integer operator*(const integer &rhs) const 
            { return {positive * rhs.positive + negative * rhs.negative, positive * rhs.negative + negative * rhs.positive}; };
            integer &operator*=(const integer &rhs)
            { return *this = operator*(*this); };

            constexpr bool operator==(const integer &rhs) const 
            { return positive + rhs.negative == negative + rhs.positive; };
            constexpr bool operator!=(const integer &rhs) const 
            { return !operator==(rhs); };

            constexpr bool operator<(const integer &rhs) const 
            { return positive + rhs.negative < negative + rhs.positive; };
            constexpr bool operator<=(const integer &rhs) const 
            { return operator==(rhs) || operator<(rhs); };
            constexpr bool operator>(const integer &rhs) const 
            { return !operator<=(rhs); };
            constexpr bool operator>=(const integer &rhs) const 
            { return !operator<(rhs); };

            natural_number positive;
            natural_number negative;
        };
    };
};
