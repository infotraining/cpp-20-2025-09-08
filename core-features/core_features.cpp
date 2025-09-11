#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <vector>
#include <string>

using namespace std::literals;

struct Person 
{
    int id;
    std::string name;
    double height;
};

namespace Explain
{
    template <typename T, typename... TArgs>
    std::unique_ptr<T> make_unique(TArgs&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<TArgs>(args)...));
    }
}

TEST_CASE("core features")
{
    Person p1{.id = 42, .name = "John", .height = 1.76};
    Person p2{.id = 42, .height = 1.76};
    //Person p3{.name = "Adam", .id = 42, .height = 1.76};

    Person p3(665, "Eva", 1.77);

    auto uptr = std::make_unique<Person>(42, "Heap", 2.12);
}