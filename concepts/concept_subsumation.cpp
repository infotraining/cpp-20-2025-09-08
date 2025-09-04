#include <catch2/catch_test_macros.hpp>
#include <concepts>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std::literals;

struct BoundingBox
{
    int w, h;
};

struct Color
{
    uint8_t r, g, b;
};

struct Rect
{
    int w, h;
    Color color;

    void draw() const
    {
        std::cout << "Rect::draw()\n";
    }

    BoundingBox box() const noexcept
    {
        return BoundingBox{w, h};
    }
};

struct ColorRect : Rect
{
    Color color;

    Color get_color() const noexcept
    {
        return color;
    }

    void set_color(Color new_color)
    {
        color = new_color;
    }
};

// clang-format off
template <typename T>
concept Shape = requires(const T& obj)
{
    { obj.box() } noexcept -> std::same_as<BoundingBox>;
    obj.draw();
};
// clang-format on

// TODO: Add concept ShapeWithColor that subsumes Shape and requires getters/setters for color

static_assert(Shape<Rect>);
static_assert(Shape<ColorRect>);

template <Shape T>
void render(T& shp)
{
    std::cout << "render<Shape T>\n";
    shp.draw();
}

// TODO: Add render function that accepts ShapeWithColor

TEST_CASE("concept subsumation")
{
    Rect r{10, 20, {255, 0, 0}};
    ColorRect cr{10, 20, {0, 255, 0}};

    render(r);
    render(cr);
}