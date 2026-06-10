//
// Created by Michael Repplinger on 10.06.26.
//

#pragma once
#include <list>

#include "clegmed/core/Consumer.hpp"

namespace clegmed::plugins::generic {

    template<typename TContainer, typename TElement>
    concept Container = requires(TContainer& container, TElement&& element)
      { container.push_back(std::forward<TElement>(element));}
      || requires(TContainer& container, TElement&& element)
      { container.push(std::forward<TElement>(element)); };


    template<typename TContainer>
    requires Container<TContainer, typename TContainer::value_type>
    [[nodiscard]] auto store(TContainer& data_storage) {
        using DataType = typename TContainer::value_type;

        // Das Lambda nutzt if constexpr, um flexibel push oder push_back zu rufen
        auto lambda_strategy = [&data_storage](const DataType& data) {
            if constexpr (requires { data_storage.push_back(data); }) {
                data_storage.push_back(data);
            } else {
                data_storage.push(data);
            }
        };

        return core::make_consumer(lambda_strategy); // Gibt die Strategie zurück
    }
}
