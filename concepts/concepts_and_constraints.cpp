#include <catch2/catch_test_macros.hpp>
#include <concepts>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <list>
#include <set>

using namespace std::literals;

template <typename TContainer>
void print(TContainer&& c, std::string_view prefix = "items")
{
    std::cout << prefix << ": [ ";
    for (const auto& item : c)
        std::cout << item << " ";
    std::cout << "]\n";
}

/////////////////////////////////////////
// IsPointer

template <typename T>
struct IsPointer
{
    static constexpr bool value = false;
};

template <typename T>
struct IsPointer<T*>
{
    static constexpr bool value = true;
};

template <typename T>
struct IsPointer<std::shared_ptr<T>> : std::true_type
{ };

template <typename T>
inline constexpr bool IsPointer_v = IsPointer<T>::value;

//////////////////////////////////////////////////////
// max_value

namespace ver_1
{
    template <typename T>
    T max_value(T a, T b)
    {
        return a < b ? b : a;
    }

    namespace BeforeCpp20
    {
        template <typename T>
        auto max_value(T a, T b)
            -> std::enable_if_t<IsPointer_v<T>, std::remove_reference_t<decltype(*a)>>
        {
            assert(a != nullptr);
            assert(b != nullptr);

            return *a < *b ? *b : *a;
        }
    } // namespace BeforeCpp20

    template <typename T>
        requires IsPointer_v<T>
    auto max_value(T a, T b)
    {
        assert(a != nullptr);
        assert(b != nullptr);

        return *a < *b ? *b : *a;
    }
} // namespace ver_1

namespace ver_2
{
    template <typename T>
    concept Pointer = IsPointer_v<T>;

    template <typename T>
    T max_value(T a, T b)
    {
        return a < b ? b : a;
    }

    template <Pointer T>
    auto max_value(T a, T b)
    {
        assert(a != nullptr);
        assert(b != nullptr);

        return *a < *b ? *b : *a;
    }
} // namespace ver_2

inline namespace ver_3
{
    template <typename T>
    concept Pointer = requires(T ptr) {
        *ptr;
        ptr == nullptr;
        ptr != nullptr;
    };

    static_assert(Pointer<int*>);
    static_assert(Pointer<const int*>);
    static_assert(!Pointer<const int>);
    static_assert(Pointer<std::shared_ptr<int>>);

    template <typename T>
    T max_value(T a, T b)
    {
        return a < b ? b : a;
    }

    template <Pointer T>
    auto max_value(T a, T b)
    {
        assert(a != nullptr);
        assert(b != nullptr);

        return *a < *b ? *b : *a;
    }
} // namespace ver_3

namespace ver_4
{
    template <typename T>
    concept Pointer = requires(T ptr) {
        *ptr;
        ptr == nullptr;
        ptr != nullptr;
    };

    auto max_value(auto a, auto b)
    {
        return a < b ? b : a;
    }

    auto max_value(Pointer auto a, Pointer auto b)
        requires // requires clause
            requires { *a < *b; } // requires expression
            && std::same_as<decltype(a), decltype(b)>
    {
        assert(a != nullptr);
        assert(b != nullptr);

        return *a < *b ? *b : *a;
    }
} // namespace ver_4

TEST_CASE("constraints")
{
    int x = 10;
    int y = 20;

    CHECK(max_value(x, y) == 20);
    CHECK(max_value("abc"s, "def"s) == "def"s);

    CHECK(ver_1::BeforeCpp20::max_value(&x, &y) == 20);
    CHECK(max_value(&x, &y) == 20);

    auto sptr_1 = std::make_shared<int>(42);
    auto sptr_2 = std::make_shared<int>(665);
    CHECK(max_value(sptr_1, sptr_2) == 665);
}

template <typename T>
concept Coutable = requires(const T& obj, std::ostream& out) {
    out << obj;
};

template <typename T>
struct Wrapper
{
    T value;

    void print() const
        requires Coutable<T>
    {
        std::cout << "Value: " << value << "\n";
    }

    void print() const 
        requires std::ranges::range<T> &&
            Coutable<std::ranges::range_value_t<T>>
    {
        std::cout << "Values: [ ";
        for(const auto& item : value)
            std::cout << item << " ";
        std::cout << "]\n";
    }
};

TEST_CASE("concepts")
{
    Wrapper<int> wrapped_int{42};
    wrapped_int.print();

    Wrapper<std::vector<int>> wrapped_vec{{1, 2, 3, 4}};
    wrapped_vec.print();

    Wrapper<std::pair<int, int>> wrapped_pair{{1, 2}};
    //wrapped_pair.print();
}

void add_to(auto& container, auto&& value)
{
    if constexpr(requires { container.push_back(std::forward<decltype(value)>(value)); })
        container.push_back(std::forward<decltype(value)>(value));
    else 
        container.insert(std::forward<decltype(value)>(value));
}

TEST_CASE("requires expression")
{
    std::vector<int> vec;
    add_to(vec, 42);

    std::set<int> my_set;
    add_to(my_set, 42);
}

template <typename T1, typename T2>
concept Addable = requires(T1 a, T2 b) {
    a + b;
};


template <typename T>
concept LeanPointer = Pointer<T> && (sizeof(T) == sizeof(void*));

static_assert(LeanPointer<int*>);
static_assert(!LeanPointer<std::shared_ptr<int>>);

struct A
{
    struct B
    {};

    using C = int;

    typedef int D;
};

template <typename T>
concept WithNestedTypes = requires {
    typename T::B; // type requirements
    typename T::C;
    typename T::D;
};

static_assert(WithNestedTypes<A>);

struct Data
{
    int size() const noexcept
    {
        return 42;
    }
};

template <typename T>
concept HasSize = requires(const T& obj) {
    { obj.size() } noexcept -> std::convertible_to<size_t>; 
};

static_assert(HasSize<Data>);
