//
// Created by Michael Repplinger on 07.06.26.
//

#pragma once

#include <type_traits>
#include <clegmed/core/Producer.hpp>

namespace clegmed::plugins::generic {

    template <typename OutputData>
    [[nodiscard]] auto emit(const OutputData& output_data) {
        auto lambda_strategy = [data = std::move(output_data)]() -> OutputData {
            return data;
        };
        return core::Producer<OutputData, decltype(lambda_strategy)>(std::move(lambda_strategy));
    }

}
