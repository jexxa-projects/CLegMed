//
// Created by Michael Repplinger on 09.06.26.
//

#pragma once
#include "../../core/Processor.hpp"

namespace clegmed::plugins::generic {

    template <typename InputData>
    [[nodiscard]] auto passThrough() {
        auto lambda_strategy = [](const InputData& input_data) -> InputData {
            return input_data;
        };

        return core::make_processor(std::move(lambda_strategy));
    }
}

