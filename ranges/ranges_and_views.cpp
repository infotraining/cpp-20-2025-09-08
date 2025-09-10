#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <deque>
#include <helpers.hpp>
#include <iostream>
#include <list>
#include <map>
#include <print>
#include <ranges>
#include <string>
#include <vector>

using namespace std::literals;

void user_foo(std::ranges::range auto& container, auto projection)
{
    using TItem = std::ranges::range_value_t<decltype(container)>;
    std::vector<TItem> vec;
}

TEST_CASE("ranges", "[ranges]")
{
    auto data = helpers::create_numeric_dataset<20>(42);
    helpers::print(data, "data");

    std::vector words = {"one"s, "two"s, "three"s, "four"s, "five"s, "six"s, "seven"s, "eight"s, "nine"s, "ten"s,
        "eleven"s, "twelve"s, "thirteen"s, "fourteen"s, "fifteen"s, "sixteen"s, "seventeen"s, "eighteen"s, "nineteen"s, "twenty"s};
    helpers::print(words, "words");

    SECTION("algorithms")
    {
        std::ranges::sort(words, std::greater<>{});
        CHECK(std::ranges::is_sorted(words, std::greater<>{}));
    }

    SECTION("projections")
    {
        // std::sort(words.begin(), words.end(),
        //           [](const auto& a, const auto& b) { return a.size() < b.size(); });

        std::ranges::sort(words, std::less{}, &std::string::size);

        helpers::print(words, "words sorted by size");
    }

    SECTION("concepts & tools")
    {
        int arr[256];

        using T = std::ranges::range_value_t<decltype(arr)>;
    }
}

template <auto Value>
struct EndValue
{
    bool operator==(auto it) const
    {
        return *it == Value;
    }
};

TEST_CASE("sentinels", "[ranges]")
{
    std::vector data = {2, 3, 4, 1, 5, 42, 6, 7, 8, 9, 10};

    // TODO - sort range [begin; 42) in descending order
    EndValue<42> my_sentinel;

    std::ranges::sort(data.begin(), my_sentinel);

    helpers::print(data, "data");

    std::contiguous_iterator auto pos = std::ranges::find(data.begin(), std::unreachable_sentinel, 42);

    CHECK(*pos == 42);
}

TEST_CASE("views")
{
    std::deque data = {2, 3, 4, 1, 5, 42, 6, 7, 8, 9, 10};

    SECTION("all")
    {
        auto all_view = std::views::all(data);
        all_view[3] = 665;

        auto another_view = all_view; // fast - O(1)
    }

    SECTION("subrange - iterator & sentinel as a view")
    {
        auto head = std::ranges::subrange(data.begin(), EndValue<42>{});

        std::ranges::sort(head);

        for (auto& item : head)
            item = 0;

        helpers::print(head, "head");
        helpers::print(data, "data");
    }

    SECTION("counted")
    {
        auto tail = std::views::counted(data.rbegin(), 3);

        std::ranges::fill(tail, 999);

        helpers::print(data, "data");
    }

    SECTION("iota")
    {
        auto gen = std::views::iota(1, 20);

        for (const auto& item : gen)
        {
            std::cout << item << " ";
        }
        std::cout << "\n";
    }

    SECTION("single")
    {
        auto one_only = std::views::single(42);

        for (const auto& item : one_only)
            std::cout << item << "\n";
    }

    SECTION("pipes |")
    {
        auto data = std::views::iota(1)
            | std::views::take(20)
            | std::views::filter([](int n) { return n % 2 == 0; })
            | std::views::transform([](int n) { return n * n; })
            | std::views::reverse;

        helpers::print(data, "data");
    }
}

TEST_CASE("views - reference semantics")
{
    std::vector data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    auto evens_view = data | std::views::filter([](int i) { return i % 2 == 0; });
    helpers::print(data, "data");

    // for(auto& item : evens_view)
    //     item = 0;

    for (auto it = evens_view.begin(); it != evens_view.end(); ++it)
    {
        auto& item = *it;
        item = 0;
    }

    // std::ranges::fill(evens_view, 0);

    helpers::print(data, "data");
    std::print("data: {}\n", data);
}

TEST_CASE("maps & ranges")
{
    std::map<int, std::string> dict = {{1, "one"}, {2, "two"}};

    helpers::print(dict | std::views::keys, "keys");
    helpers::print(dict | std::views::values, "values");

    auto keys_of_dict = dict | std::views::elements<0>;
    helpers::print(keys_of_dict, "keys_of_dict");

    for (const auto& key : dict | std::views::keys)
    {
        std::cout << key << "\n";
    }
}

TEST_CASE("split")
{
    std::string text = "abc def ghi";

    auto tokens_view = std::views::split(text, " ")
        | std::views::transform([](auto token) { return std::string_view(token); })
        | std::ranges::to<std::vector>();

    helpers::print(tokens_view, "tokens");
}

TEST_CASE("enumerate")
{
    std::vector vec = {"zero", "one", "two", "three", "four"};

    for (const auto& [index, value] : std::views::enumerate(vec))
        std::cout << index << ": " << value << "\n";
    
    std::cout << "\n";

    auto mapped = vec | std::views::enumerate | std::ranges::to<std::map>();

    for (const auto& [index, value] : mapped)
        std::cout << index << ": " << value << "\n";
}