#include <iostream>

import Primes; // importing module Primes

int main()
{
    std::cout << "check if 13 is prime: " << is_prime(13) << "\n";
    // std::cout << "check if 42 is prime: " << IsPrime{}(42) << "\n";

    std::cout << "First 10 primes: ";
    for (const auto& n : get_primes_vec(10))
        std::cout << n << " ";
    std::cout << "\n";
}