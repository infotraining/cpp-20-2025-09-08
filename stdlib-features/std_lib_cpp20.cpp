#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <span>
#include <string>
#include <vector>
#include <algorithm>
#include <numbers>
#include <mdspan>
#include <print>

using namespace std::literals;

void zero(std::span<int> sp, int default_value = 0)
{
    for(auto& item : sp)
        item = default_value;
}

void print(std::span<const int> sp)
{
    std::cout << "[ ";
    for(auto& item : sp)
        std::cout << item << " ";
    std::cout << "]";
}

TEST_CASE("std::span")
{
    int buffer[256];

    SECTION("compile time extent")
    {
        std::span buffer_span_1{buffer};
        CHECK(buffer_span_1.size() == 256);

        zero(buffer_span_1);
    }

    SECTION("dynamic extent")
    {
        std::span<int> buffer_span_2{buffer};
        CHECK(buffer_span_2.size() == 256);

        zero(buffer_span_2);
        CHECK(std::ranges::all_of(buffer, [](int x) { return x == 0;}));

        zero(buffer);
        CHECK(std::ranges::all_of(buffer, [](int x) { return x == 0;}));

        std::vector<int> vec = {1, 2, 3, 4};
        zero(std::span{vec.data() + 1, 2});
        CHECK(vec == std::vector{1, 0, 0, 4});

        print(vec);
    }
}

TEST_CASE("BEWARE! Dangling pointers!!!")
{
    std::vector<int> vec = {1, 2, 3, 4};
    std::span sp_vec{vec};

    print(sp_vec);

    vec.push_back(5);

    print(sp_vec);
}

void print_as_bytes(const float f, const std::span<const std::byte> bytes)
{
#ifdef __cpp_lib_format
    std::cout << std::format("{:+6}", f) << " - { ";

    for (std::byte b : bytes)
    {
        std::cout << std::format("{:02X} ", std::to_integer<int>(b));
    }

    std::cout << "}\n";
#endif
}

namespace Explain
{
    template <typename T>
    constexpr static T Pi_v(3.141592653589793);
}

TEST_CASE("float as span of bytes")
{
    float data[] = {std::numbers::pi_v<float>};
    //float data[] = {Explain::Pi_v<float>};

    std::span<const std::byte> const_bytes = std::as_bytes(std::span{data});
    print_as_bytes(data[0], const_bytes);

    std::span<std::byte> writeble_bytes = std::as_writable_bytes(std::span{data});
    writeble_bytes[3] |= std::byte{0b1000'0000};
    print_as_bytes(data[0], const_bytes);
}

TEST_CASE("mdspan")
{
    std::vector v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

    // View data as contiguous memory representing 2 rows of 6 ints each
    auto ms2 = std::mdspan(v.data(), 2, 6);
    
    // View the same data as a 3D array 2 x 3 x 2
    auto ms3 = std::mdspan(v.data(), 2, 3, 2);

    // Write data using 2D view
    for (std::size_t i = 0; i != ms2.extent(0); i++)
        for (std::size_t j = 0; j != ms2.extent(1); j++)
            ms2[i, j] = i * 1000 + j;

    // Read back using 3D view
    for (std::size_t i = 0; i != ms3.extent(0); i++)
    {
        std::println("slice @ i = {}", i);
        for (std::size_t j = 0; j != ms3.extent(1); j++)
        {
            for (std::size_t k = 0; k != ms3.extent(2); k++)
                std::print("{} ", ms3[i, j, k]);
            std::println("");
        }
    }
}

TEST_CASE("vector never shrins implcitly")
{
    std::vector<int> vec;
    CHECK(vec.size() == 0);
    CHECK(vec.capacity() == 0);

    for(int i = 0; i < 1'000'000; ++i)
        vec.push_back(i);
    CHECK(vec.size() == 1'000'000);
    CHECK(vec.capacity() >= 1'000'000);

    vec.clear();
    CHECK(vec.size() == 0);
    vec.shrink_to_fit();
    CHECK(vec.capacity() == 0);
}