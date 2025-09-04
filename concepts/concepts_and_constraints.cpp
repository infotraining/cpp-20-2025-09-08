#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <concepts>

using namespace std::literals;

template <typename TContainer>
void print(TContainer&& c, std::string_view prefix = "items")
{
    std::cout << prefix << ": [ ";
    for(const auto& item : c)
        std::cout << item << " ";
    std::cout << "]\n";
}

template <typename T>
T max_value(T a, T b)
{
    return a < b ? b : a;
}

TEST_CASE("constraints")
{
    int x = 10;
    int y = 20;

    CHECK(max_value(x, y) == 20);

    //CHECK(max_value(&x, &y) == 20);
}

TEST_CASE("concepts")
{
    REQUIRE(true);
}