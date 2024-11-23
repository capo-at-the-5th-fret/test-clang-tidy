#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include "caff/cmath.h"
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

TEMPLATE_LIST_TEST_CASE("evenly_divisible", "[cmath][even_divisible]",
    caff::standard_integer_types)
{
    using test_types_t = evenly_divisible_test_types<TestType>;
    using caff::evenly_divisible;

    void* raw_ptr = nullptr;
    int* int_ptr = (int*)raw_ptr;

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
