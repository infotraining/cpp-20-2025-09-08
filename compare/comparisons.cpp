#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

struct Point
{
    int x;
    int y;

    friend std::ostream& operator<<(std::ostream& out, const Point& p)
    {
        return out << std::format("Point({},{})", p.x, p.y);
    }

    bool operator==(const Point& other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const
    {
        return !(*this == other);
    }
};

struct Point3D
{
    // TODO
};

TEST_CASE("Point - operator ==")
{
    SECTION("Point")
    {
        Point p1{1, 2};
        Point p2{1, 2};
        Point p3{2, 1};

        CHECK(p1 == p2);
        CHECK(p1 != p3);
    }

    // SECTION("Point3D")
    // {
    //     Point3D p1{1, 2, 3};
    //     Point3D p2{1, 2, 3};
    //     Point3D p3{1, 2, 4};

    //     CHECK(p1 == p2);
    //     CHECK(p1 != p3);
    // }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Comparisons
{

    struct Money
    {
        int dollars;
        int cents;

        constexpr Money(int dollars, int cents)
            : dollars(dollars)
            , cents(cents)
        {
            if (cents < 0 || cents > 99)
            {
                throw std::invalid_argument("cents must be between 0 and 99");
            }
        }

        constexpr Money(double amount)
            : dollars(static_cast<int>(amount))
            , cents(static_cast<int>(amount * 100) % 100)
        { }

        friend std::ostream& operator<<(std::ostream& out, const Money& m)
        {
            return out << std::format("${}.{}", m.dollars, m.cents);
        }

        // TODO: add operator <=>
    };

    namespace Literals
    {
        // clang-format off
        constexpr Money operator""_USD(long double amount)
        {
            return Money(amount);
        }
        // clang-format on
    } // namespace Literals
} // namespace Comparisons

TEST_CASE("Money - operator <=>")
{
    using Comparisons::Money;
    using namespace Comparisons::Literals;

    Money m1{42, 50};
    Money m2{42, 50};

    SECTION("comparison operators are synthetized")
    {
        // CHECK(m1 == m2);
        // CHECK(m1 == Money(42.50));
        // CHECK(m1 == 42.50_USD);
        // CHECK(m1 != 42.51_USD);
        // CHECK(m1 < 42.51_USD);
        // CHECK(m1 <= 42.51_USD);
        // CHECK(m1 > 0.99_USD);
        // CHECK(m1 >= 0.99_USD);

        // static_assert(Money{42, 50} == 42.50_USD);
    }

    SECTION("sorting")
    {
        // std::vector<Money> wallet{42.50_USD, 13.37_USD, 0.99_USD, 100.00_USD, 0.01_USD};
        // std::ranges::sort(wallet);
        // CHECK(std::ranges::is_sorted(wallet));
    }
}

TEST_CASE("operator <=>")
{
    SECTION("primitive types")
    {
        int x = 42;

        // TODO: add checks
    }

    SECTION("custom types")
    {
        SECTION("result is a comparison category")
        {
            // TODO
        }

        SECTION("operators <, >, <=, >= are synthetized")
        {
            // TODO
        }
    }
}

TEST_CASE("comparison categories")
{
    SECTION("strong ordering")
    {
        // TODO
    }

    SECTION("partial ordering")
    {
        // TODO
    }

    SECTION("weak ordering")
    {
        // TODO
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Comparisons
{
    class Data
    {
        int* buffer_;
        size_t size_;

    public:
        Data(std::initializer_list<int> values)
            : buffer_(new int[values.size()])
            , size_(values.size())
        {
            std::copy(values.begin(), values.end(), buffer_);
        }

        ~Data()
        {
            delete[] buffer_;
        }

        // TODO: add comparisons operators
    };
} // namespace Comparisons

TEST_CASE("lexicographical_compare_three_way")
{
    using Comparisons::Data;

    Data data1{1, 2, 3};
    Data data2{1, 2, 3};
    Data data3{1, 2, 4};
}