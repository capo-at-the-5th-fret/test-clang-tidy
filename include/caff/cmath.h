#pragma once

#include <cmath>
#include <tuple>
#include "caff/extra.h"

namespace caff
{
    template <standard_integer T, standard_integer U>
    constexpr bool evenly_divisible(T t, U u)
    {
        return (t % u) == 0;
    }
}
