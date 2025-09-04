export module Shapes:Rectangle;

import std;

import :Base;
import :Point;

export namespace Shapes
{
    class Rectangle : public ShapeBase
    {
        int width_, height_;

    public:
        static constexpr const char* id = "Rectangle";

        Rectangle(int x = 0, int y = 0, int w = 0, int h = 0);

        int width() const
        {
            return width_;
        }

        void set_width(int w)
        {
            width_ = w;
        }

        int height() const
        {
            return height_;
        }

        void set_height(int h)
        {
            height_ = h;
        }

        void draw() const override;
    };
} // namespace Shapes

namespace Shapes
{
    Rectangle::Rectangle(int x, int y, int w, int h)
        : ShapeBase{x, y}
        , width_{w}
        , height_{h}
    { }

    void Rectangle::draw() const
    {
        std::cout << "Drawing rectangle at " << coord() << " with width: " << width_
                  << " and height: " << height_ << std::endl;
    }
} // namespace Shapes