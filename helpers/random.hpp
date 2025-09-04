#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <utility>
#include <cstdint>
#include <numeric>

namespace helpers::random
{
    struct PCG
    {
        struct pcg_32t_random_t
        {
            std::uint64_t state = 0;
            std::uint64_t inc = 0;
        };

        pcg_32t_random_t rng;

        using result_type = std::uint32_t;

        constexpr explicit PCG(std::uint64_t seed) : rng{.state=seed}
        {            
        }

        constexpr result_type operator()()
        {
            return pcg32_random_r();
        }

        static constexpr result_type min()
        {
            return std::numeric_limits<result_type>::min();
        }

        static constexpr result_type max()
        {
            return std::numeric_limits<result_type>::max();
        }

    private:
        constexpr std::uint32_t pcg32_random_r()
        {
            std::uint64_t old_state = rng.state;

            // advance internal state
            rng.state = old_state * 6364126223846793005ULL + (rng.inc | 1);

            // calculate output function (XHS RR), uses old state for max ILP
            std::uint32_t xor_shifted = ((old_state >> 18u) ^ old_state) >> 27u;
            std::uint32_t rot = old_state >> 59u;

            return (xor_shifted >> rot) | (xor_shifted << ((-rot) & 31));
        }
    };
} // namespace helpers::random

#endif