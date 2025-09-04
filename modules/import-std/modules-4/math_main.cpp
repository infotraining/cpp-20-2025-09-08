import std;

import Math; // importing module Math

int main()
{
    std::cout << "check if 13 is prime: " << Math::Primes::is_prime(13) << "\n";
    // std::cout << "check if 42 is prime: " << IsPrime{}(42) << "\n";

    using Math::Primes::get_primes, Math::Primes::first_primes;

    std::cout << "First 10 primes: ";
    for (const auto& n : get_primes<10>())
        std::cout << n << " ";
    std::cout << "\n";

    // const std::array my_primes = get_primes<20>(); // ERROR - std::array is not visible
    const auto my_primes = get_primes<20>(); // but reachable

    std::cout << "My primes: ";
    for (const std::uint32_t& n : my_primes)
        std::cout << n << " ";
    std::cout << "\n";

    std::cout << "First primes: ";
    for (const auto& n : first_primes )
        std::cout << n << " ";
    std::cout << "\n";

    std::cout << "Fibonacci lookup table: ";
    for(const auto& fib : Math::Fibonacci::fibonacci_lookup_table | std::views::take(15))
        std::cout << fib << " ";
    std::cout << "...\n";
}