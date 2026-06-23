#pragma once

#include <clegmed/core/Producer.hpp>

namespace clegmed::plugins::generic {

    template <typename OutputData>
    [[nodiscard]] auto emit(OutputData output_data) {

        using PipelineType = std::conditional_t<
            std::is_array_v<OutputData> || std::is_same_v<std::decay_t<OutputData>, const char*>,
            std::string,
            std::decay_t<OutputData>
        >;

        auto lambda_strategy = [data = PipelineType(std::move(output_data))]() -> PipelineType {
            return data; // Gibt nun legal einen std::string zurück
        };

        return core::make_producer(std::move(lambda_strategy));
    }
}
