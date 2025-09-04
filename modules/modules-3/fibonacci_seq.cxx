module;

#include <cstdint>
#include <array>

export module Math:Fibonacci;

export namespace Math::Fibonacci // all declarations in this namespace are exported
{
    constexpr uint32_t fibonacci(uint32_t n)
    {
        if (n <= 1)
        {
            return n;
        }

        return fibonacci(n - 1) + fibonacci(n - 2);
    }

    template <uint32_t N>
    constexpr std::array<uint32_t, N> get_fibonacci_sequence()
    {
        std::array<uint32_t, N> fibonaccis{};

        for (uint32_t i = 0; i < N; ++i)
        {
            fibonaccis[i] = fibonacci(i);
        }

        return fibonaccis;
    }

    constexpr std::array fibonacci_lookup_table = get_fibonacci_sequence<20>();
}