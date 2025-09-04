module; // global fragment module

#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <ranges>
#include <algorithm>

export module Primes; // declaration of module Primes - primary module interface

export constexpr bool is_prime(uint32_t n)
{
    if (n <= 1)
        return false;

    for (auto i = 2u; i < n; ++i)
    {
        if (n % i == 0)
            return false;
    }

    return true;
}

struct IsPrime
{
    bool operator()(uint32_t n) const
    {
        return is_prime(n);
    }
};

export template <uint32_t N>
constexpr std::array<uint32_t, N> get_primes()
{
    std::array<uint32_t, N> primes{};

    auto primes_view = std::views::iota(2u) | std::views::filter(is_prime) | std::views::take(N);

    std::ranges::copy(primes_view, primes.begin());

    return primes;
}

export constexpr std::array first_primes = get_primes<100>();