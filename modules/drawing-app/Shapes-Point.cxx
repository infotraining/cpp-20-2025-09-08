module;

#include <iostream>

export module Shapes:Point;

export namespace Shapes
{
    struct Point
    {
        int x = 0;
        int y = 0;

        constexpr Point(int x = 0, int y = 0) noexcept
            : x{x}
            , y{y}
        { }

        constexpr void translate(int dx, int dy) noexcept
        {
            x += dx;
            y += dy;
        }
    };

    std::ostream& operator<<(std::ostream& out, const Point& pt);

    std::istream& operator>>(std::istream& in, Point& pt);
} // namespace Shapes

static constexpr const char opening_bracket = '[';
static constexpr const char closing_bracket = ']';
static constexpr const char comma = ',';

namespace Shapes
{
    std::ostream& operator<<(std::ostream& out, const Point& pt)
    {
        out << opening_bracket << pt.x << comma << pt.y << closing_bracket;
        return out;
    }

    std::istream& operator>>(std::istream& in, Point& pt)
    {
        char start, separator, end;
        int x, y;

        if (in >> start && start != opening_bracket)
        {
            in.unget();
            in.clear(std::ios_base::failbit);
            return in;
        }

        in >> x >> separator >> y >> end;

        if (!in || (separator != comma) || (end != closing_bracket))
            throw std::runtime_error("Stream reading error");

        pt.x = x;
        pt.y = y;

        return in;
    }
} // namespace Shapes