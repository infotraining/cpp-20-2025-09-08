#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <utility>

using namespace std::literals;

auto my_comparer(auto a, auto b) 
{
    if constexpr(std::integral<decltype(a)> 
        && std::integral<decltype(b)>)
        return std::cmp_less(a, b);
    return a < b;
}

TEST_CASE("safe comparing integral numbers")
{
    int x = -42;
    unsigned int y = 665;

    SECTION("direct comparison")
    {
        CHECK(std::cmp_less(x, y));
        CHECK(std::in_range<int>(y));
        CHECK(std::in_range<unsigned char>(x) == false);

        //CHECK(std::cmp_greater("one"s, "two"s));
    }

    SECTION("with generic functions")
    {
        auto my_comparer = [](auto a, auto b) {
            if constexpr(std::integral<decltype(a)> 
                && std::integral<decltype(b)>)
                return std::cmp_less(a, b);
            return a < b;
        };

        CHECK(my_comparer(x, y));
        CHECK(my_comparer("one"s, "two"s));
    }
}