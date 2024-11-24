#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include "caff/cmath.h"
#include <fmt/core.h>
//#include "caff/type_list.h"

TEST_CASE("foo", "[cmath]")
{
    CHECK(caff::foo(1) == 123);
}

namespace
{
    // clang-format off
    template <typename T>
    using evenly_divisible_test_types =
        std::conditional_t<
            std::is_same_v<T, int>,
            std::tuple<std::tuple<int, int>>,   // only need to test int,int,
                                                // the others are redundant
            std::tuple
            <
                std::tuple<T,T>,                // test T,T/T,int/int,T
                std::tuple<T,int>,
                std::tuple<int,T>
            >
        >;
    // clang-format on
}

#if 0
TEMPLATE_LIST_TEST_CASE("evenly_divisible using signed types",
    "[cmath][even_divisible]", caff::signed_standard_integer_types)
{
    using T = TestType;
    static_assert(std::is_signed_v<T>);

    using x_type = T;
    using y_type = T;

    const auto [x, y, expected] = GENERATE(table<x_type, y_type, bool>({

        // positive, positive
        { x_type{ 0 }, y_type{ 3 }, true },
        { x_type{ 1 }, y_type{ 3 }, false },
        { x_type{ 2 }, y_type{ 3 }, false },
        { x_type{ 3 }, y_type{ 3 }, true },
        { x_type{ 4 }, y_type{ 3 }, false },
        { x_type{ 5 }, y_type{ 3 }, false },
        { x_type{ 6 }, y_type{ 3 }, true },

        // positive, negative
        { x_type{ 0 }, y_type{ -3 }, true },
        { x_type{ 1 }, y_type{ -3 }, false },
        { x_type{ 2 }, y_type{ -3 }, false },
        { x_type{ 3 }, y_type{ -3 }, true },
        { x_type{ 4 }, y_type{ -3 }, false },
        { x_type{ 5 }, y_type{ -3 }, false },
        { x_type{ 6 }, y_type{ -3 }, true },

        // negative, positive
        { x_type{ -0 }, y_type{ 3 }, true },
        { x_type{ -1 }, y_type{ 3 }, false },
        { x_type{ -2 }, y_type{ 3 }, false },
        { x_type{ -3 }, y_type{ 3 }, true },
        { x_type{ -4 }, y_type{ 3 }, false },
        { x_type{ -5 }, y_type{ 3 }, false },
        { x_type{ -6 }, y_type{ 3 }, true },

        // negative, negative
        { x_type{ -0 }, y_type{ -3 }, true },
        { x_type{ -1 }, y_type{ -3 }, false },
        { x_type{ -2 }, y_type{ -3 }, false },
        { x_type{ -3 }, y_type{ -3 }, true },
        { x_type{ -4 }, y_type{ -3 }, false },
        { x_type{ -5 }, y_type{ -3 }, false },
        { x_type{ -6 }, y_type{ -3 }, true }

    }));

    CAPTURE(x, y, expected);
    CHECK(caff::evenly_divisible(x, y) == expected);
}

TEMPLATE_LIST_TEST_CASE("evenly_divisible using unsigned types",
    "[cmath][even_divisible]", caff::unsigned_standard_integer_types)
{
    using T = TestType;
    static_assert(std::is_unsigned_v<T>);

    using x_type = T;
    using y_type = T;

    const auto [x, y, expected] = GENERATE(table<T,T,bool>({
        { x_type{ 0 }, y_type{ 3 }, true },
        { x_type{ 1 }, y_type{ 3 }, false },
        { x_type{ 2 }, y_type{ 3 }, false },
        { x_type{ 3 }, y_type{ 3 }, true },
        { x_type{ 4 }, y_type{ 3 }, false },
        { x_type{ 5 }, y_type{ 3 }, false },
        { x_type{ 6 }, y_type{ 3 }, true }
    }));

    CAPTURE(x, y, expected);
    CHECK(caff::evenly_divisible(x, y) == expected);
}
#endif

TEMPLATE_LIST_TEST_CASE("evenly_divisible", "[cmath][even_divisible]",
    caff::unsigned_standard_integer_types)
{
    using test_types_t = evenly_divisible_test_types<TestType>;
    using caff::evenly_divisible;

    caff::tuple_for_each_type<test_types_t>([]<typename T>
    {
        using x_type = std::tuple_element_t<0,T>;
        using y_type = std::tuple_element_t<1,T>;

        const auto [x, y, expected] = GENERATE(table<x_type,y_type,bool>({
            { x_type{ 0 }, y_type{ 3 }, true },
            { x_type{ 1 }, y_type{ 3 }, false },
            { x_type{ 2 }, y_type{ 3 }, false },
            { x_type{ 3 }, y_type{ 3 }, true },
            { x_type{ 4 }, y_type{ 3 }, false },
            { x_type{ 5 }, y_type{ 3 }, false },
            { x_type{ 6 }, y_type{ 3 }, true }
        }));

        //fmt::println("{} ({}) {} ({}) {}", x, typeid(x_type).name(), y,
          //  typeid(y_type).name(), expected);
        //CAPTURE(x, y, expected);
        //CHECK(evenly_divisible(x, y) == expected);
        /*if constexpr (std::is_signed_v<TestType>)
        {
            CHECK_FALSE(evenly_divisible(x_type{-1}, rhs));
            CHECK_FALSE(evenly_divisible(x_type{-2}, rhs));
            CHECK(evenly_divisible(x_type{-3}, rhs));
            CHECK_FALSE(evenly_divisible(x_type{-4}, rhs));
            CHECK_FALSE(evenly_divisible(x_type{-5}, rhs));
            CHECK(evenly_divisible(x_type{-6}, rhs));
        }*/
    }); 
}

#if 0
TEMPLATE_LIST_TEST_CASE("evenly_divisible", "[cmath][even_divisible]",
    caff::standard_integer_types)
{
    using test_types_t = evenly_divisible_test_types<TestType>;
    using caff::evenly_divisible;

    caff::tuple_for_each_type<test_types_t>([]<typename T>
    {
        using x_type = std::tuple_element_t<0,T>;
        using y_type = std::tuple_element_t<1,T>;

        const y_type rhs{ 3 };
        CHECK(evenly_divisible(x_type{0}, rhs));
        CHECK_FALSE(evenly_divisible(x_type{1}, rhs));
        CHECK_FALSE(evenly_divisible(x_type{2}, rhs));
        CHECK(evenly_divisible(x_type{3}, rhs));
        CHECK_FALSE(evenly_divisible(x_type{4}, rhs));
        CHECK_FALSE(evenly_divisible(x_type{5}, rhs));
        CHECK(evenly_divisible(x_type{6}, rhs));
        
        if constexpr (std::is_signed_v<TestType>)
        {
            CHECK_FALSE(evenly_divisible(x_type{-1}, rhs));
            CHECK_FALSE(evenly_divisible(x_type{-2}, rhs));
            CHECK(evenly_divisible(x_type{-3}, rhs));
            CHECK_FALSE(evenly_divisible(x_type{-4}, rhs));
            CHECK_FALSE(evenly_divisible(x_type{-5}, rhs));
            CHECK(evenly_divisible(x_type{-6}, rhs));
        }
    }); 
}
#endif