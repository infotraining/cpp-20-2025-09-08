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

    bool operator==(const Point& other) const = default;

    // bool operator!=(const Point& other) const
    // {
    //     return !(*this == other);
    // }
};

bool operator==(const Point& p1, const std::pair<int, int>& p2)
{
    return p1.x == p2.first && p1.y == p2.second;
}

struct Point3D : Point
{
    int z;

    constexpr Point3D(int x, int y, int z) : Point{x, y}, z{z}
    {}

    bool operator==(const Point3D& other) const = default;
};

TEST_CASE("Point - operator ==")
{
    SECTION("Point")
    {
        Point p1{1, 2};
        Point p2{1, 2};
        Point p3{2, 1};

        CHECK(p1 == p2);
        CHECK(p1 != p3); // rewirite to !(p1 == p2)

        CHECK(p1 == std::pair{1, 2});
        CHECK(std::pair{1, 2} == p1);
        CHECK(std::pair{1, 2} != p3);
    }

    SECTION("Point3D")
    {
        Point3D p1{1, 2, 3};
        Point3D p2{1, 2, 3};
        Point3D p3{1, 2, 4};

        CHECK(p1 == p2);
        CHECK(p1 != p3);
    }
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

        auto operator<=>(const Money& other) const = default;
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
        CHECK(m1 == m2);
        CHECK(m1 == Money(42.50));
        CHECK(m1 == 42.50_USD);
        CHECK(m1 != 42.51_USD);
        CHECK(m1 < 42.51_USD);
        CHECK(m1 <= 42.51_USD);
        CHECK(m1 > 0.99_USD);
        CHECK(m1 >= 0.99_USD);

        static_assert(Money{42, 50} == 42.50_USD);

        SECTION("it is possible to compare with integral type")
        {
            bool they_are_equal = m1 <=> m2 == 0;
            CHECK(they_are_equal);

            bool left_is_less = m1 <=> 42.51_USD < 0;
            CHECK(left_is_less);
        }

        SECTION("we should prefer common syntax")
        {
            bool they_are_equal = m1 == m2;
            CHECK(they_are_equal);

            bool left_is_less = m1 < 42.51_USD; // (m1 <=> 42.51_USD < 0)
            CHECK(left_is_less);
        }
    }

    SECTION("sorting")
    {
        std::vector<Money> wallet{42.50_USD, 13.37_USD, 0.99_USD, 100.00_USD, 0.01_USD};
        //std::ranges::sort(wallet);
        std::sort(wallet.begin(), wallet.end(), std::less<>{});
        CHECK(std::ranges::is_sorted(wallet));

        std::vector<double> vec_that_floats{3.14, 5.77, 
                                            std::numeric_limits<double>::quiet_NaN(), 88.66};
        std::sort(vec_that_floats.begin(), vec_that_floats.end(), 
                  [](auto a, auto b) { return std::strong_order(a, b) < 0; });

        std::cout << std::format("sorted: {}", vec_that_floats) << "\n";
    }
}

TEST_CASE("operator <=>")
{
    SECTION("primitive types")
    {
        int x = 42;

        auto result = x <=> 42;
        static_assert(std::is_same_v<decltype(result), std::strong_ordering>);
        CHECK(result == std::strong_ordering::equal);
    }

    SECTION("custom types")
    {
        using Comparisons::Money;

        SECTION("result is a comparison category")
        {
            CHECK((Money{40, 10} <=> Money{40, 10} == 0));
        }

        SECTION("operators <, >, <=, >= are synthetized")
        {
            CHECK(Money{40, 10} < Money{40, 20});
        }
    }
}

TEST_CASE("comparison categories")
{
    SECTION("strong ordering")
    {
        auto result = 42 <=> 665;
        static_assert(std::is_same_v<decltype(result), std::strong_ordering>);
    }

    SECTION("partial ordering")
    {
        double x = 3.14;
        auto result = x <=> std::numeric_limits<double>::quiet_NaN();
        //auto unordered_to_zero = std::partial_ordering::unordered == 0;
        //CHECK(unordered_to_zero == false);
        CHECK(x <=> std::numeric_limits<double>::quiet_NaN() 
              == std::partial_ordering::unordered);;

        static_assert(std::is_same_v<decltype(result), std::partial_ordering>);
        CHECK((result == std::partial_ordering::unordered));
    }

    SECTION("weak ordering")
    {
        
    }
}

////////////////////////////////////////////////////////////////////////////////////
struct Temperature
{
    double value;

    auto operator<=>(const Temperature& other) const
    {
        return std::strong_order(value, other.value);
    }

    bool operator==(const Temperature& other) const
    {
        return std::strong_order(value, other.value) == 0;
    }
};

struct PreCpp20
{
    int value;

    // bool operator==(const PreCpp20& other) const
    // {
    //     return value == other.value;
    // }

    // bool operator<(const PreCpp20& other) const
    // {
    //     return value < other.value;
    // }

    auto operator<=>(const PreCpp20& other) const = default;
};

struct PostCpp20
{
    int id;
    PreCpp20 member;

    auto operator<=>(const PostCpp20& other) const = default;
};

TEST_CASE("custom types - <=>")
{
    Temperature t1{30.1};
    Temperature t2{30.1};
    Temperature t3{45.2};

    CHECK(t1 == t2);
    CHECK(t1 < t3);
    CHECK(t3 >= t1);

    PostCpp20 pc1{42, PreCpp20{665}};
    PostCpp20 pc2{42, PreCpp20{42}};

    CHECK(pc1 > pc2);
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

        auto operator<=>(const Data& other) const
        {
            return std::lexicographical_compare_three_way(
                buffer_, buffer_ + size_, other.buffer_, other.buffer_ + other.size_);
        }

        bool operator==(const Data& other) const
        {
            if (size_ != other.size_)
                return false;

            return std::equal(buffer_, buffer_ + size_, other.buffer_);
        }
     };
} // namespace Comparisons

TEST_CASE("lexicographical_compare_three_way")
{
    using Comparisons::Data;

    Data data1{1, 2, 3};
    Data data2{1, 2, 3};
    Data data3{1, 2, 4};

    CHECK(data1 == data2);
    CHECK(data1 < data3);
}

class Car
{
    std::string licence_plate_;
    int milage_;
public:
    Car(const std::string& licence_plate, int milage) : licence_plate_{licence_plate}, milage_{milage}
    {}

    std::string get_licence_plate() const
    {
        return licence_plate_;
    }

    int get_milage()
    {
        return milage_;
    }

    void drive(int distance)
    {
        milage_ += distance;
    }

    std::weak_ordering operator<=>(const Car& other) const
    {
        return licence_plate_ <=> other.licence_plate_;
    }

    bool operator==(const Car& other) const
    {
        return licence_plate_ == other.licence_plate_;
    }
};