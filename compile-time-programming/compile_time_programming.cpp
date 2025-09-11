#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <array>
#include <ranges>
#include <algorithm>

using namespace std::literals;

int runtime_func(int x)
{
    return x * x;
}

constexpr int constexpr_func(int x)
{
    return x * x;
}

consteval int consteval_func(int x)
{
    return x * x;
}

TEST_CASE("consteval")
{
    int x = 4;
    
    SECTION("runtime call")
    {
        REQUIRE(runtime_func(x) == 16);
        REQUIRE(constexpr_func(x) == 16);
        //REQUIRE(consteval_func(x) == 16);
    }

    SECTION("compile-time call")
    {
        constexpr auto x = 4;
        //static_assert(runtime_func(x) == 16);
        static_assert(constexpr_func(x) == 16);
        static_assert(consteval_func(x) == 16);
    }

    SECTION("immidiete function")
    {
        constexpr int cx = consteval_func(4);
        int x = consteval_func(5); // int x = 25;
        int y = runtime_func(5);
        int z = constexpr_func(5);

        constexpr std::array squares = { consteval_func(1), consteval_func(2), consteval_func(3) };
        // std::array squares = { 1, 4, 9 };
    }
}

void compile_time_error() // runtime func
{}
 
consteval int next_two_digit_value(int value)
{
    if (value < 9 || value >= 99)
    {
        compile_time_error();
        //throw std::out_of_range("Arg out of range");
    }
 
    return ++value;
}

constexpr int len(const char* s)
{
    //if (std::is_constant_evaluated()) // C++20
    if consteval // C++23
    {
        // compile-time friendly code
        int idx = 0;

        while (s[idx] != '\0')
            ++idx;
        return idx;
    }
    else
    {
        return std::strlen(s); // function called at runtime
    }
}

TEST_CASE("consteval functions")
{
    static_assert(next_two_digit_value(55) == 56);

    const char* ctext = "Hello";
    REQUIRE(len(ctext) == 5);

    constexpr auto compile_time_ctext = "Hello";
    static_assert(len(compile_time_ctext) == 5);
}

constexpr int fibonacci(int n)
{
    if (n <= 1)
        return n;
    return fibonacci(n-1) + fibonacci(n-2);
}

template <size_t N, typename F>
consteval auto create_lookup_table(F func)
{
    std::array<uintmax_t, N> values{};

    auto func_values = 
        std::views::iota(0)
        | std::views::take(N)
        | std::views::transform([func](auto n) { return func(n); });

    std::ranges::copy(func_values, values.begin());

    return values;
}

constexpr int fast_fibonacci(int n)
{
    constexpr auto lookup_fib = create_lookup_table<20>(fibonacci);

    if (n < std::size(lookup_fib))
    {
        return lookup_fib[n];
    }
    else
        return fibonacci(n);
}

TEST_CASE("lookup tables")
{
    REQUIRE(fibonacci(10) == 55);
    REQUIRE(fibonacci(11) == 89);
    static_assert(fibonacci(10) == 55);\
    
    constexpr auto lookup_table = create_lookup_table<20>(fibonacci);

    static_assert(fast_fibonacci(10) == 55);
}