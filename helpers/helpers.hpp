#ifndef HELPERS_HPP
#define HELPERS_HPP

#include "random.hpp"

#include <iostream>
#include <random>
#include <ranges>
#include <algorithm>
#include <utility>
#include <cstdint>

namespace helpers
{
    template <typename T>
    concept PrintableRange = std::ranges::range<T> && requires(std::ranges::range_value_t<T>&& item) { std::cout << item; };

    void print(PrintableRange auto&& rng, std::string_view prefix = "rng")
    {
        std::cout << prefix << " = [ ";
        for (const auto& item : rng)
        {
            if constexpr (std::convertible_to<decltype(item), std::string_view>)
            {
                std::cout << '"' << item << '"' << " ";
            }
            else
            {
                std::cout << item << " ";
            }
        }
        std::cout << "]\n";
    }

    template <size_t Size>
    [[nodiscard]] constexpr auto create_numeric_dataset(uint32_t seed = 42, int low = -100, int high = 100)
    {
        std::vector<int> data(Size);
        data.reserve(Size);

        auto uniform_distr = [low, high](auto&& rnd_gen) {
            uint32_t width = high - low;
            return (rnd_gen() % width) + low;
        };

        if (std::is_constant_evaluated())
        {
            random::PCG pcg_rnd{seed};

            std::ranges::generate(data, [&] { return uniform_distr(pcg_rnd); });
        }
        else
        {
            std::mt19937 mt_rnd{seed};

            std::ranges::generate(data, [&] { return uniform_distr(mt_rnd); });
        }

        std::array<int, Size> result_data{};
        std::ranges::copy(data, result_data.begin());

        return result_data;
    }
} // namespace helpers

#endif