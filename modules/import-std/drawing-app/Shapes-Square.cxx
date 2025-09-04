module;

#include <cassert>

export module Shapes:Square;

import :Base;
import :Rectangle;
import :Point;

namespace Shapes
{
    export class Square : public Shape
    {
        Rectangle rect_;

    public:
        static constexpr const char* id = "Square";

        Square(int x = 0, int y = 0, int size = 0);

        Point coord() const;

        void set_coord(const Point& pt);

        int size() const;

        void set_size(int size);

        void draw() const override;

        void move(int dx, int dy) override;
    };

    Square::Square(int x, int y, int size)
        : rect_{x, y, size, size}
    {
    }

    void Square::move(int dx, int dy)
    {
        rect_.move(dx, dy);
    }

    Point Square::coord() const
    {
        return rect_.coord();
    }

    void Square::set_coord(const Point& pt)
    {
        rect_.set_coord(pt);
    }

    int Square::size() const
    {
        return rect_.height();
    }

    void Square::set_size(int size)
    {
        rect_.set_width(size);
        rect_.set_height(size);

        assert(rect_.width() == rect_.height());
    }

    void Square::draw() const
    {
        rect_.draw();
    }

} // namespace Shapes