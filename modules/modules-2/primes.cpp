module; // global fragment module

#include <cstdint>
#include <vector>
#include <ranges>

module Primes; // module implementation unit

bool IsPrime::operator()(uint32_t n) const
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

std::vector<uint32_t> get_primes_vec(uint32_t n)
{
    auto primes_view = std::views::iota(2u) | std::views::filter(is_prime) | std::views::take(n) | std::views::common;

    std::vector<uint32_t> primes(primes_view.begin(), primes_view.end());

    return primes;
}
