export module Math:Primes;

import std;

namespace Math::Primes
{
    export constexpr bool is_prime(std::uint32_t n)
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

    export template <std::uint32_t N>
    constexpr std::array<std::uint32_t, N> get_primes()
    {
        std::array<std::uint32_t, N> primes{};

        auto primes_view = std::views::iota(2u) | std::views::filter(is_prime) | std::views::take(N);

        std::ranges::copy(primes_view, primes.begin());

        return primes;
    }

    export constexpr std::array first_primes = get_primes<100>();
}