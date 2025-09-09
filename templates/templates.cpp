#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <string>
#include <vector>

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
    Explain::add_to(words, token);     // passing lvalue

    Explain::add_to(words, "Hello"s);  // passing rvalue (prvalue)

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