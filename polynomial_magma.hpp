#pragma once

#include "./utility.hpp"

#include <algorithm>
#include <numeric>
#include <vector>

namespace bbb {
    namespace math {
        template <typename CoefficientField>
        struct polynomial {
            using cofficient_field_type = CoefficientField;
        };
    };
};