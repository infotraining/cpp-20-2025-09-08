export module Math:Fibonacci;

import std;

export namespace Math::Fibonacci // all declarations in this namespace are exported
{
    constexpr std::uint32_t fibonacci(std::uint32_t n)
    {
        if (n <= 1)
        {
            return n;
        }

        return fibonacci(n - 1) + fibonacci(n - 2);
    }

    template <std::uint32_t N>
    constexpr std::array<std::uint32_t, N> get_fibonacci_sequence()
    {
        std::array<std::uint32_t, N> fibonaccis{};

        for (std::uint32_t i = 0; i < N; ++i)
        {
            fibonaccis[i] = fibonacci(i);
        }

        return fibonaccis;
    }

    constexpr std::array fibonacci_lookup_table = get_fibonacci_sequence<20>();
}