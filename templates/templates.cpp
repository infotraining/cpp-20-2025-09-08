#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <array>

using namespace std::literals;

auto add(auto a, auto b)
{
    return a + b;
}

namespace Explain
{
    template <typename T1, typename T2>
    auto add(T1 a, T2 b)
    {
        return a + b;
    }
} // namespace Explain

TEST_CASE("templates & lambda expressions")
{
    auto generic_add = [](auto a, auto b) {
        return a + b;
    };

    REQUIRE(generic_add(4, 7) == 11);
    REQUIRE(add("Hello"s, "World") == "HelloWorld");
}

void add_to(auto& container, auto&& value)
{
    container.push_back(std::forward<decltype(value)>(value));
}

/////////////////////////////////////////////////////////////
// Universal reference & perfect forwarding

namespace Explain
{
    namespace Step_1
    {
        void add_to(std::vector<std::string>& container, const std::string& str)
        {
            container.push_back(str);
        }

        void add_to(std::vector<std::string>& container, std::string&& str)
        {
            container.push_back(std::move(str));
        }
    } // namespace Step_1

    template <typename TArg>
    void add_to(auto& container, TArg&& arg /* universal reference */)
    {
        container.push_back(std::forward<TArg>(arg));
    }
} // namespace Explain

template <typename TContainer>
void zero(TContainer& container)
{
    for (auto&& item : container)
        item = 0;
}

TEST_CASE("add_to container")
{
    std::vector<std::string> words;

    std::string token = "World";
    Explain::add_to(words, token); // passing lvalue

    Explain::add_to(words, "Hello"s); // passing rvalue (prvalue)

    Explain::add_to(words, std::move(token)); // passing rvalue (xvalue)
}

TEST_CASE("auto&& - universal reference")
{
    SECTION("auto&& declaration")
    {

        auto&& pi_1 = 3.14; // universal reference

        double PI = 3.14;
        auto&& pi_2 = PI; // universal reference
    }

    SECTION("range-based for")
    {
        std::vector<bool> vec = {true, false, true};

        for (auto&& item : vec)
            item = false;

        SECTION("is interpreted")
        {
            for (auto it = vec.begin(); it != vec.end(); ++it)
            {
                auto&& item = *it;
                item = false;
            }
        }
    }
}

///////////////////////////////////////////////////////
// NTTP - Non Type Template Params

template <double Factor, typename T>
auto scale(T x)
{
    return Factor * x;
}

TEST_CASE("double as NTTP")
{
    REQUIRE(scale<2.0>(8) == 16.0);
}

struct Tax
{
    double value;

    constexpr Tax(double v)
        : value{v}
    { }

    double get_value() const
    {
        return value;
    }
};

template <Tax Vat>
constexpr auto calc_gross_price(double net_price)
{
    return net_price + net_price * Vat.get_value();
}

TEST_CASE("structural types as NTTP")
{
    constexpr Tax vat_pl{0.23};
    constexpr Tax vat_es{0.20};

    REQUIRE(calc_gross_price<vat_pl>(100.0) == 123.0);
    REQUIRE(calc_gross_price<vat_es>(100.0) == 120.0);
}

template <size_t N>
struct StaticString
{
    char text[N];

    constexpr StaticString(const char (&str)[N]) noexcept
    {
        std::copy(str, str + N, text);
    }

    auto operator<=>(const StaticString& other) const = default;

    friend std::ostream& operator<<(std::ostream& out, const StaticString& str)
    {
        out << str.text;
        return out;
    }
};

template <StaticString LoggerName>
struct Logger
{
public:
    void log(std::string_view msg)
    {
        std::cout << LoggerName << ": " << msg << "\n";
    }
};

TEST_CASE("strings as NTTP")
{
    StaticString str("text");

    Logger<"main_logger"> logger_1;
    Logger<"low_priority_logger"> logger_2;

    logger_1.log("Start");
    logger_2.log("Start");
}

///////////////////////////////////////////
// lambda as NTTP

template <std::invocable auto GetVat>
constexpr auto calc_gross_price_with_lambda(double net_price)
{
    return net_price + net_price * GetVat();
}

TEST_CASE("lambdas as NTTP")
{
    constexpr auto get_vat_pl = [] {
        return 0.23;
    };
    constexpr auto get_vat_es = [] {
        return 0.20;
    };

    REQUIRE(calc_gross_price_with_lambda<get_vat_pl>(100.0) == 123.0);
    REQUIRE(calc_gross_price_with_lambda<get_vat_es>(100.0) == 120.0);
    REQUIRE(calc_gross_price_with_lambda<[] {
        return 0.01;
    }>(100.0)
        == 101.0);
}

void foo(int x)
{ 
    std::cout << std::format("foo({})\n", x);
}

template <typename T>
void deduce(T arg)
{
}

namespace Cpp20
{
    void deduce(auto arg)
    {
    }
} // namespace Cpp20

TEST_CASE("quiz")
{
    int x = 42;
    const int cx = x;
    int& ref_x = x;
    const int& ref_cx = x;
    int tab[10];

    {
        auto ax1 = x;      // int
        auto ax2 = cx;     // int - const is removed
        auto ax3 = ref_x;  // int - ref is removed
        auto ax4 = ref_cx; // int - ref-to-const is removed
        auto ax5 = tab;    // int*
        auto ax6 = foo;    // void(*)(int)
    }

    {
        auto& ax1 = x;      // int&
        auto& ax2 = cx;     // const int& - const is not removed
        auto& ax3 = ref_x;  // int& - ref is removed
        auto& ax4 = ref_cx; // const int& - ref-to-const is not removed
        auto& ax5 = tab;    // int(&ax5)[10]
        auto& ax6 = foo;    // void(&)(int)
    }

    {
        auto&& ax1 = std::string{"text"}; // std::string&&

        std::string text{"text"};
        auto&& ax2 = text;                // std::string&
    }
}

//////////////////////////////////////////////////
// Lambdas in C++20

TEST_CASE("explicit template params")
{
    auto add_one = []<typename T>(const std::vector<T>& vec) { 
        vec.push_back(1);
    };

    auto to_array = []<typename T, size_t N>(T (&native_array)[N]) {
        std::array<T, N> arr{};
        std::ranges::copy(native_array, arr.begin());

        return arr;
    };

    int tab[10];
    auto arr = to_array(tab);
}

int add_int(int a, int b)
{
    return a + b;
}

std::string add_str(const std::string& a, const std::string& b)
{
    return a + b;
}

struct Foo
{
    void operator()(int x) const
    {
        std::cout << std::format("Foo::op({})\n", x);
    }
};

std::string& get_nth(std::vector<std::string>& vec, size_t index)
{
    return vec[index];
}

TEST_CASE("get_nth")
{
    std::vector<std::string> words = {"one", "tow"};

    REQUIRE(get_nth(words, 0) == "one");

    get_nth(words, 1) = "two";
    REQUIRE(words[1] == "two");
}

template <typename F, typename... TArgs>
decltype(auto) call(F func, TArgs&&... args) // TArgs&& are universal reference
{
    Logger<"func-logger"> logger;
    logger.log("Function called!!!");

    return func(std::forward<TArgs>(args)...);
}

TEST_CASE("call wrapper that logs")
{
    call(foo, 42);
    REQUIRE(call(add_int, 30, 90) == 120) ;
    REQUIRE(call(add_str, "hello", "world") == "helloworld");

    Foo foobar;
    call(foobar, 665);
    REQUIRE(call([](int a, int b, int c) { return a + b + c; }, 1, 2, 3) == 6);

    std::vector<std::string> words = {"one", "tow"};

    call(get_nth, words, 1) = "two";
    REQUIRE(words[1] == "two");
}

TEST_CASE("default constructor for lambda")
{
    auto cmp_by_val = [](auto a, auto b) { return *a < *b; };

    using LambdaType = decltype(cmp_by_val);
    LambdaType another_cmp_by_value{};
}

//////////////////////////////////////////////////////

auto create_caller(auto f, auto... args)
{
   return [f, ...args = std::move(args)]() -> decltype(auto) {
        return f(args...);
   }; 
}

TEST_CASE("lambda - capturing argument pack")
{
    auto f = create_caller(std::plus<int>{}, 3, 5);

    REQUIRE(f() == 8);
}
