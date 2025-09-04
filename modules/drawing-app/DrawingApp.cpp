#include <iostream>
#include <memory>

import Shapes;

int main()
{
    Shapes::ShapeFactory& shape_factory = Shapes::SingletonShapeFactory::instance();

    shape_factory.register_creator(Shapes::Rectangle::id, [] { return std::make_unique<Shapes::Rectangle>(); });
    shape_factory.register_creator(Shapes::Square::id, [] { return std::make_unique<Shapes::Square>(); });

    auto rect = shape_factory.create(Shapes::Rectangle::id);
    rect->draw();

    Shapes::Square sq{0, 200, 100};
    sq.draw();
    sq.move(50, 20);
    sq.draw();
}