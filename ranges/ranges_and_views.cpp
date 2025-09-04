#include <catch2/catch_test_macros.hpp>
#include <helpers.hpp>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

using namespace std::literals;

TEST_CASE("ranges", "[ranges]")
{
    auto data = helpers::create_numeric_dataset<20>(42);
    helpers::print(data, "data");

    std::vector words = {"one"s, "two"s, "three"s, "four"s, "five"s, "six"s, "seven"s, "eight"s, "nine"s, "ten"s, 
                         "eleven"s, "twelve"s, "thirteen"s, "fourteen"s, "fifteen"s, "sixteen"s, "seventeen"s, "eighteen"s, "nineteen"s, "twenty"s};
    helpers::print(words, "words");

    SECTION("algorithms")
    {
        // TODO
    }

    SECTION("projections")
    {
        // TODO        
    }

    SECTION("concepts & tools")
    {
        // TODO
    }
}

template <auto Value>
struct EndValue
{
    bool operator==(auto it) const
    {
        return *it == Value;
    }
};

TEST_CASE("sentinels", "[ranges]")
{
    std::vector data = {2, 3, 4, 1, 5, 42, 6, 7, 8, 9, 10};

    // TODO - sort range [begin; 42) in descending order

    helpers::print(data, "data");
}

TEST_CASE("views")
{
    std::vector data = {2, 3, 4, 1, 5, 42, 6, 7, 8, 9, 10};

    SECTION("all")
    {
    }

    SECTION("subrange - iterator & sentinel as a view")
    {
    }

    SECTION("counted")
    {        
    }

    SECTION("iota")
    {
    }

    SECTION("pipes |")
    {
    }
}

TEST_CASE("views - reference semantics")
{    
    std::vector data = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    auto evens_view = data | std::views::filter([](int i) { return i % 2 == 0; });
    helpers::print(data, "data");

    // TODO - set all even numbers to 0 using evens_view

    helpers::print(data, "data");
}