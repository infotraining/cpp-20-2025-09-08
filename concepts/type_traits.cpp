#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <string>

int identity(int x)
{
    return x;
}

// Traits that return value

template <typename T>
struct SizeOf
{
    constexpr static size_t value = sizeof(T);
};

template <typename T>
constexpr size_t SizeOf_v = SizeOf<T>::value;

// Trait can return type

template<typename T>
struct Identity
{
    using type = T;
};

template <typename T>
using Identity_t = typename Identity<T>::type;

template<typename T1, typename T2>
struct IsSame
{
    constexpr static bool value = false;
};

template <typename T>
struct IsSame<T, T>
{
    constexpr static bool value = true;
};

template <typename T1, typename T2>
constexpr bool IsSame_v = IsSame<T1, T2>::value;

//////////////////////////////////
// IsVoid trait

// template<typename T>
// struct IsVoid{
//     static constexpr bool value = false;
// };

// template<>
// struct IsVoid<void>
// { 
//     static constexpr bool value = true;
// };

template <typename T, T v>
struct IntegralConstant
{
    constexpr static T value = v;
};

static_assert(IntegralConstant<int, 5>::value == 5);

template <bool v>
using BooleanConstant = IntegralConstant<bool, v>;

static_assert(BooleanConstant<true>::value == true);

using FalseType = BooleanConstant<false>;
using TrueType = BooleanConstant<true>;

template<typename T>
struct IsVoid : std::false_type {};

template<>
struct IsVoid<void> : std::true_type {};

template <typename T>
constexpr bool IsVoid_v = IsVoid<T>::value;

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
inline constexpr bool IsPointer_v = IsPointer<T>::value;

TEST_CASE("simplest traits")
{
    static_assert(SizeOf<char>::value == 1);
    static_assert(SizeOf_v<char> == 1);

    int arr[SizeOf_v<std::string>];

    int x = 42;
    static_assert(IsSame<int,decltype(x)>::value);
    static_assert(IsSame_v<int, Identity<int>::type>);
}

TEST_CASE("type traits")
{
    SECTION("IsVoid")
    {        
        static_assert(IsVoid<int>::value == false);
        static_assert(IsVoid_v<void> == true);
        static_assert(std::is_void_v<void> == true);
    }

    SECTION("IsPointer")
    {
        static_assert(IsPointer<int>::value == false);
        static_assert(IsPointer<int*>::value == true);
        static_assert(IsPointer_v<const int*> == true);
        static_assert(IsPointer_v<int****> == true);
    }
}

// RemoveConst trait

template <typename T>
struct RemoveConst
{
    using type = T;
};

template <typename T>
struct RemoveConst<const T>
{
    using type = T;
};

template <typename T>
using RemoveConst_t = typename RemoveConst<T>::type;

// RemoveReference
template <typename T>
struct RemoveReference
{
    using type = T;
};

template <typename T>
struct RemoveReference<T&>
{
    using type = T;
};

template <typename T>
struct RemoveReference<T&&>
{
    using type = T;
};

template <typename T>
using RemoveReference_t = typename RemoveReference<T>::type;


TEST_CASE("traits that transform a type")
{
    SECTION("RemoveConst")
    {
        const int cx = 42;
        static_assert(std::is_same_v<RemoveConst_t<decltype(cx)>, int>);
    }

    SECTION("RemoveReference")
    {
        static_assert(std::is_same_v<RemoveReference_t<int&>, int>);
        static_assert(std::is_same_v<RemoveReference_t<int&&>, int>);
    }
}