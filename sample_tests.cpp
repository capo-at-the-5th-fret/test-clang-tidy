#include <catch2/catch_all.hpp>

int foo(int x)
{
    int y = x + 1;
    return 123;
}

TEST_CASE("Sample test", "[sample]")
{
    CHECK(foo(1) == 123);
}