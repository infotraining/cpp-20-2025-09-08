#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <format>

using namespace std::literals;

void print(std::string_view format_sv, const auto&... args)
{
    std::cout << std::vformat(format_sv, std::make_format_args(args...)) << "\n";
}

struct Rating
{
    int value;

    bool operator==(const Rating& other) const
    {
        std::cout << std::format("Rating({}).op==({})\n", value, other.value);
        return value == other.value;
    }

    bool operator<(const Rating& other) const
    {
        std::cout << std::format("Rating({}).op<({})\n", value, other.value);
        return value < other.value;
    }
};

struct Gadget
{
    std::string name;
    double price;

    std::strong_ordering operator<=>(const Gadget& other) const
    {
        std::cout << std::format("Gadget({}, {}).op<=>(Gadget({}, {}))\n", name, price, other.name, other.price);

        if (auto cmp_result = name <=> other.name; cmp_result != 0)
            return cmp_result;
        return std::strong_order(price, other.price);
    }

    bool operator==(const Gadget& other) const = default;
};

struct SuperGadget : Gadget
{
    Rating rating;

    std::strong_ordering operator<=>(const SuperGadget& other) const
    {
        if (auto cmp_result = (*this).Gadget::operator<=>(other); cmp_result != 0)
            return cmp_result;
        return std::compare_strong_order_fallback(rating, other.rating);
    }

    bool operator==(const SuperGadget& other) const = default;
};

TEST_CASE("Gadget - write custom operator <=> - stronger category than auto detected")
{
    SECTION("==")
    {
        CHECK(Gadget{"ipad", 1.0} == Gadget{"ipad", 1.0});
    }
    
    SECTION("<=>")
    {
        CHECK(Gadget{"ipad", 1.0} <=> Gadget{"ipad", 1.0} == std::strong_ordering::equal);
    }
}

TEST_CASE("SuperGadget - write custom operator <=> - member without compare-three-way operator")
{
    CHECK(SuperGadget{{"ipad", 1.0}, Rating{1}} != SuperGadget{{"ipad", 1.0}, Rating{2}});
    CHECK(SuperGadget{{"ipad", 1.0}, Rating{1}} <=> SuperGadget{{"ipad", 1.0}, Rating{2}} == std::strong_ordering::less);    
}


//////////////////////////////////////////////////////////////////////////////////////////////////////


enum class RatingValue : uint8_t { very_poor = 1, poor, satisfactory, good, very_good, excellent};

struct RatingStar
{
public:
    RatingValue value;

    explicit RatingStar(RatingValue rating_value) : value{rating_value}
    {} 

    auto operator<=>(const RatingStar& other) const = default;

    constexpr auto operator<=>(const RatingValue& other) const noexcept
    {
        return value <=> other;
    }
};

TEST_CASE("Rating Star - implement nedded <=>")
{
    RatingStar r1{RatingValue::good};
    
    CHECK(r1 == RatingStar{RatingValue::good});
    CHECK(r1 <=> RatingStar{RatingValue::excellent} == std::strong_ordering::less);
    CHECK(r1 <=> RatingValue::excellent == std::strong_ordering::less);
}