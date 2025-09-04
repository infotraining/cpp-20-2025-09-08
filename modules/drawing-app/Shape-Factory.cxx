export module Shapes:Factory;

import Factory;
import Singleton;
import :Base;

namespace Shapes
{
    export using ShapeFactory = GenericFactory<Shape>;

    export using SingletonShapeFactory = Singleton::SingletonHolder<ShapeFactory>;
} // namespace Shapes