#pragma once

namespace bbb {
    namespace math {
        template <typename natural_number>
        struct integer {
            constexpr integer(natural_number positive, natural_number negative)
            : positive{positive < negative ? 0 : positive - negative}
            , negative{positive < negative ? negative - positive : 0}
            {};
            constexpr integer(natural_number positive)
            : integer{positive, 0}
            {};
            constexpr integer()
            : integer{0}
            {};

            constexpr integer operator+()
            { return {positive, negative}; };
            constexpr integer operator-()
            { return {negative, positive}; };

            natural_number positive;
            natural_number negative;
        };
    };
};
