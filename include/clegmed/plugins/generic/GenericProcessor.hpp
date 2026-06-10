//
// Created by Michael Repplinger on 09.06.26.
//

#pragma once
#include <concepts>
#include <string>
#include <vector>
#include "../../core/Processor.hpp"

namespace clegmed::plugins::generic {

    template<typename TInput, typename TSuffix>
    concept CanAppend =
    (
        requires(TInput& input, TSuffix&& suffix) { { input += std::forward<TSuffix>(suffix) }; } ||
        std::convertible_to<TSuffix, TInput>
    )
    &&(!std::is_arithmetic_v<std::decay_t<TInput>> && !std::is_arithmetic_v<std::decay_t<TSuffix>>);

    template<typename TInput = void, typename TSuffix>
    [[nodiscard]] auto append(TSuffix suffix) {

        // HIER DIE KORREKTUR:
        // Wenn TInput void ist, prüfen wir: Ist TSuffix ein C-String (Zeiger auf const char)?
        // Wenn ja, nutzen wir std::string als Pipeline-Typ. Wenn nein, den Typ selbst.
        using DeducedInput = std::conditional_t<
            std::is_same_v<std::decay_t<TSuffix>, const char*>,
            std::string,
            std::decay_t<TSuffix>
        >;

        using ActualInputType = std::conditional_t<std::is_same_v<TInput, void>, DeducedInput, TInput>;

        // Concept-Check
        static_assert(CanAppend<ActualInputType, TSuffix>,
            "Typen sind nicht fuer Append kompatibel oder es handelt sich um Zahlen!");

        // Das Lambda arbeitet nun sicher mit std::string (wenn Text übergeben wurde)
        auto lambda_strategy = [suffix = std::move(suffix)](ActualInputType input_data) -> ActualInputType {
            input_data += suffix; // Funktioniert jetzt perfekt, da std::string += const char* erlaubt ist!
            return input_data;
        };

        return core::make_processor(lambda_strategy);
    }

    /**
     * Passthrough filter
     * @tparam InputData
     * @return
     */
    template <typename InputData>
    [[nodiscard]] auto passThrough() {
        auto lambda_strategy = [](const InputData& input_data) -> InputData {
            return input_data;
        };

        return core::make_processor(std::move(lambda_strategy));
    }
}

