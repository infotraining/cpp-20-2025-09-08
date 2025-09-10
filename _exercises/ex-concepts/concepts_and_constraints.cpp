#include <catch2/catch_test_macros.hpp>
#include <forward_list>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <source_location>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std::literals;

constexpr static bool TODO = false;

/*********************
Iterator concept
1. iterator is dereferenceable: *iter
2. can be pre-incremented - returns reference to iterator
3. can be post-incremented
4. supports == and !=
**********************/

template <typename I>
concept Iterator = requires(I iterator) {
    iterator++;
    { ++iterator } -> std::same_as<I&>;
    *iterator;
} && std::equality_comparable<I>;

TEST_CASE("Iterator - concept")
{
    // uncomment when concept is implemented
    static_assert(Iterator<std::vector<int>::iterator>);
    static_assert(Iterator<std::vector<int>::const_iterator>);
    static_assert(Iterator<std::list<int>::iterator>);
    static_assert(Iterator<int*>);
    static_assert(Iterator<const int*>);
}

////////////////////////////////////////////////////////////////////////

/*********************
StdContainer concept
1. std::begin(C&) returns iterator
2. std::end(C&) returns iterator
**********************/

template <typename Container>
concept StdContainer = requires(Container& c) {
    { std::begin(c) } -> Iterator;
    { std::end(c) } -> Iterator;
};

namespace Explain
{
    template <typename Container>
    concept range = requires(Container& c) {
        std::ranges::begin(c);
        std::ranges::end(c);
    };
} // namespace Explain

TEST_CASE("StdContainer - concept")
{
    // uncomment when concept is implemented
    static_assert(StdContainer<std::vector<int>>);
    static_assert(StdContainer<std::list<int>>);
    static_assert(StdContainer<int[10]>);
    static_assert(StdContainer<std::string>);
    static_assert(StdContainer<std::forward_list<int>>);
}

////////////////////////////////////////////////////////////////////////

/*********************
SizedContainer concept
1. is StdContainer
2. std::size(C&) returns T convertible to size_t
**********************/

template <typename Container>
concept SizedContainer = StdContainer<Container> && requires(Container& c) {
    { std::size(c) } -> std::convertible_to<size_t>;
};

TEST_CASE("SizedContainer - concept")
{
    // uncomment when concept is implemented
    static_assert(SizedContainer<std::vector<int>>);
    static_assert(SizedContainer<int[10]>);
    static_assert(SizedContainer<std::string>);
    static_assert(!SizedContainer<std::forward_list<int>>);
}

////////////////////////////////////////////////////////////////////////

/*********************
 * Indexable concept
 1. can be indexed: c[index]
*********************/

struct CrazyContainer
{
    //using key_type = std::vector<int>;

    int operator[](std::vector<int> key)
    {
        return 42;
    }
};

template <typename T>
concept HasKeyType = requires {
    typename T::key_type;
};

template <typename T>
struct IndexType 
{
    using type = size_t;
};

template <typename T>
    requires HasKeyType<T>
struct IndexType<T> {
    using type = typename T::key_type;
};

template <>
struct IndexType<CrazyContainer>
{
    using type = std::vector<int>;
};

template <typename T>
using IndexType_t = typename IndexType<T>::type;

// template <typename C>
// concept Indexable = requires(C& c, size_t index) {c[index]; } 
//     || requires(C& c, typename C::key_type index) { c[index]; };


template <typename C>
concept Indexable = requires(C& c, IndexType_t<C> index) {
    c[index];
};


TEST_CASE("Indexable - concept")
{
    // uncomment when concept is implemented
    static_assert(Indexable<std::vector<int>>);
    static_assert(Indexable<int[10]>);
    static_assert(Indexable<std::string>);
    static_assert(Indexable<std::unique_ptr<int[]>>);
    static_assert(!Indexable<std::forward_list<int>>);
    static_assert(!Indexable<std::list<int>>);
    static_assert(Indexable<std::map<int, std::string>>);

    SECTION("extra case - for ambitious")
    {
        static_assert(Indexable<std::map<std::string, std::string>>);  // EXTRA

        std::map<int, int> m1;
        m1[8] = 6;

        std::map<std::string, int> m2;
        m2["key"] = 5;

        static_assert(Indexable<CrazyContainer>);
    }
}

/*********************
IndexableContainer concept
1. is SizedContainer
2. is Indexable
**********************/

template <typename C>
concept IndexableContainer = SizedContainer<C> && Indexable<C>;

TEST_CASE("IndexableContainer - concept")
{
    // uncomment when concept is implemented
    static_assert(IndexableContainer<std::vector<int>>);
    static_assert(!IndexableContainer<std::list<int>>);
    static_assert(!IndexableContainer<std::set<int>>);
    static_assert(IndexableContainer<std::map<int, std::string>>);
    static_assert(IndexableContainer<std::unordered_map<int, int>>);
    static_assert(IndexableContainer<std::vector<bool>>);
    static_assert(IndexableContainer<std::string>);
    static_assert(IndexableContainer<int[256]>);
}

// uncomment when concept is implemented
void print_all(const StdContainer auto& container)
{
    std::cout << "void print_all(const StdContainer auto& container)\n";

    for (const auto& item : container)
    {
        std::cout << item << " ";
    }
    std::cout << "\n";
}

// uncomment when concept is implemented
void print_all(const IndexableContainer auto& container)
{
    std::cout << "void print_all(const IndexableContainer auto& container)\n";

    for (size_t i = 0; i < std::size(container); ++i)
    {
        std::cout << container[i] << " ";
    }
    std::cout << "\n";
}

TEST_CASE("container concepts")
{
    // uncomment when concept is implemented
    std::vector vec = {1, 2, 3, 4};
    print_all(vec);

    std::list lst{1, 2, 3};
    print_all(lst);
}

/////////////////////////////////////////////////////////////////////////////////////
// TODO: add constraints to the algorithm
// template<typename T>
// concept _Range_ = requires(T& rng) {

//     std::ranges::begin(rng);
//     std::ranges::end(rng);
//     std::ranges::range_value_t<T>{};
    
// };

template <std::ranges::range TRng>
    requires std::default_initializable<std::ranges::range_value_t<TRng>>
void zero(TRng& rng)
{
    using TValue = std::ranges::range_value_t<TRng>;

    for (auto&& item : rng)
        item = TValue{};
}

TEST_CASE("zero")
{
    SECTION("vector<int>")
    {
        std::vector<int> vec = {1, 2, 3};
        zero(vec);
        CHECK(vec == std::vector{0, 0, 0});
    }

    SECTION("list<std::string>")
    {
        std::list<std::string> lst = {"one", "two", "three"};
        zero(lst);
        CHECK(lst == std::list{""s, ""s, ""s});
    }

    SECTION("vector<bool>")
    {
        std::vector<bool> evil_vec_bool = {true, false, true};
        zero(evil_vec_bool);
        CHECK(evil_vec_bool == std::vector{false, false, false});
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////

SCENARIO("std::vector")
{
    GIVEN("a std::vector")
    {
        std::vector<int> vec = {1, 2, 3};

        REQUIRE(vec.size() == 3);

        WHEN("push_back is called")
        {
            vec.push_back(4);

            THEN("size increases by one")
            {
                REQUIRE(vec.size() == 4);
            }
        }

        WHEN("pop_back is called")
        {
            vec.pop_back();

            THEN("size decreases by one")
            {
                REQUIRE(vec.size() == 2);
            }
        }
    }

    // Tear down
}
