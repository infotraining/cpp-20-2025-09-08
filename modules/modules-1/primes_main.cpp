#include <cstdint>
#include <iostream>

import Primes; // importing module Primes

int main()
{
    std::cout << "check if 13 is prime: " << is_prime(13) << "\n";
    // std::cout << "check if 42 is prime: " << IsPrime{}(42) << "\n";

    std::cout << "First 10 primes: ";
    for (const auto& n : get_primes<10>())
        std::cout << n << " ";
    std::cout << "\n";

    // const std::array my_primes = get_primes<20>(); // ERROR - std::array is not visible
    const auto my_primes = get_primes<20>(); // but reachable

    std::cout << "My primes: ";
    for (const uint32_t& n : my_primes)
        std::cout << n << " ";
    std::cout << "\n";

    std::cout << "First primes: ";
    for (const uint32_t& n : first_primes)
        std::cout << n << " ";
    std::cout << "\n";
}