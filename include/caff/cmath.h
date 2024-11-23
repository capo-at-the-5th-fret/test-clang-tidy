#pragma once

#include <cmath>
#include <tuple>
#include "caff/extra.h"

namespace caff
{
    inline int foo(int x)
    {
        int y = x + 1;
        return 123;
    }

    template <standard_integer T, standard_integer U>
    constexpr bool evenly_divisible(T t, U u)
    {
        return (t % u) == 0;
    }
}
