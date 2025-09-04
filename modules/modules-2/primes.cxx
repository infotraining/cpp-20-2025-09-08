module; // global fragment module

#include <array>
#include <cmath>
#include <cstdint>
#include <vector>

export module Primes; // declaration of module Primes - primary module interface unit

export struct IsPrime;

struct IsPrime
{
    constexpr IsPrime() = default;
    bool operator()(uint32_t n) const;
};

export std::vector<uint32_t> get_primes_vec(uint32_t n);

export constinit auto is_prime = IsPrime{};