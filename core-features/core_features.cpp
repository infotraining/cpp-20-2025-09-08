#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

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
} // namespace Explain

TEST_CASE("core features")
{
    Person p1{.id = 42, .name = "John", .height = 1.76};
    Person p2{.id = 42, .height = 1.76};
    // Person p3{.name = "Adam", .id = 42, .height = 1.76};

    Person p3(665, "Eva", 1.77);

    auto uptr = std::make_unique<Person>(42, "Heap", 2.12);
}

///////////////////////////////////////////////////////
// C++23

namespace BeforeCpp23
{
    struct Less
    {
        bool operator()(int a, int b) const
        {
            return a < b;
        }
    };
} // namespace BeforeCpp23

namespace Cpp23
{
    struct Less
    {
        static bool operator()(int a, int b)
        {
            return a < b;
        }
    };
} // namespace Cpp23

TEST_CASE("static operator()")
{
    Cpp23::Less less;

    REQUIRE(less(2, 6) == true);

    auto static_lambda = []() static {
        return 42;
    };

    decltype(static_lambda) another_lambda;
    REQUIRE(another_lambda() == 42);
}

class Cell
{
    std::string value_;

public:
    Cell(std::string value) noexcept
        : value_{std::move(value)}
    { }

    void set_value(this Cell& self, int v)
    {
        self.value_ = std::to_string(v);
    }

    // std::string& get_value() &
    // {
    //     return value_;
    // }

    // const std::string& get_value() const&
    // {
    //     return value_;
    // }

    // std::string&& get_value() &&
    // {
    //     return std::move(value_);
    // }

    template <typename TSelf>
    auto&& get_value(this TSelf&& self)
    {
        return std::forward<TSelf>(self).value_;
    }

    void bar(this Cell& self) // &
    {
        std::cout << "Called for lvalue\n";
    }

    void bar(this const Cell& self) // const&
    {
        std::cout << "Called for lvalue-const\n";
    }

    void bar(this Cell&& self) // &&
    {
        std::cout << "called for rvalue - temporary\n";
    }

    auto foo() -> std::string
    {
        return "foo";
    }
};

TEST_CASE("explicit this")
{
    Cell c1{"42"};

    const Cell c2{"665"};

    c1.bar();
    c2.bar();
    Cell{"9"}.bar();

    auto var = c2.get_value();
    auto var2 = Cell{"123"}.get_value();
}

int foo_add(int a, int b);
auto foo_add(int a, int b) -> int;


TEST_CASE("recursive lambda")
{
    auto factorial = [](this auto &&self, int n) -> int
    {
        return n == 0 ? 1 : n * self(n - 1);
    };

    CHECK(factorial(5) == 120);
}

TEST_CASE("arrow")
{
    auto describe = [](int x) -> std::string {
        if (x % 2 == 0)
            return std::string("even");
        return "odd";
    };

    CHECK(describe(4) == "even"s);
}