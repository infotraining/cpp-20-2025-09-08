#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <format>

using namespace std::literals;

struct Rating // Don't change this struct
{
    int value;

    bool operator==(const Rating& other) const
    {
        return value == other.value;
    }

    bool operator<(const Rating& other) const
    {
        return value < other.value;
    }
};

struct Gadget
{
    std::string name;
    double price;

    // TODO
};

struct SuperGadget : Gadget
{
    Rating rating;

    // TODO
};

TEST_CASE("Gadget - write custom operator <=> - stronger category than auto detected")
{
    // SECTION("==")
    // {
    //     CHECK(Gadget{"ipad", 1.0} == Gadget{"ipad", 1.0});
    // }
    
    // SECTION("<=>")
    // {
    //     static_assert(std::is_same_v<decltype(Gadget{"ipad", 1.0} <=> Gadget{"ipad", 1.0}), std::strong_ordering>);
        
    //     CHECK(Gadget{"ipad", 1.0} <=> Gadget{"ipad", 1.0} == std::strong_ordering::equal);
    // }
}

TEST_CASE("SuperGadget - write custom operator <=> - member without compare-three-way operator")
{
    // CHECK(SuperGadget{{"ipad", 1.0}, Rating{1}} != SuperGadget{{"ipad", 1.0}, Rating{2}});
    // CHECK(SuperGadget{{"ipad", 1.0}, Rating{1}} <=> SuperGadget{{"ipad", 1.0}, Rating{2}} == std::strong_ordering::less);    
}


//////////////////////////////////////////////////////////////////////////////////////////////////////

enum class RatingValue : uint8_t { very_poor = 1, poor, satisfactory, good, very_good, excellent};

struct RatingStar
{
public:
    RatingValue value;

    explicit RatingStar(RatingValue rating_value) : value{rating_value}
    {} 

    // TODO
};

TEST_CASE("Rating Star - implement needed <=>")
{
    RatingStar r1{RatingValue::good};
    
    // CHECK(r1 == RatingStar{RatingValue::good});
    // CHECK(r1 <=> RatingStar{RatingValue::excellent} == std::strong_ordering::less);
    // CHECK(r1 <=> RatingValue::excellent == std::strong_ordering::less);
    // CHECK(RatingValue::excellent <=> r1 == std::strong_ordering::greater);
}