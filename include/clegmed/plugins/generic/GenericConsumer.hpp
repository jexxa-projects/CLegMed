#pragma once
#include <list>

#include "clegmed/core/Consumer.hpp"
#include "clegmed/utils/Logger.hpp"


namespace clegmed::plugins::generic {

    template<typename TContainer, typename TElement>
    concept Container = requires(TContainer& container, TElement&& element)
      { container.push_back(std::forward<TElement>(element));}
      || requires(TContainer& container, TElement&& element)
      { container.push(std::forward<TElement>(element)); };

    template<typename T>
    [[nodiscard]] auto discard() {
        return core::make_consumer([](T ) {
            // we discard data
        });
    }


    template<typename TContainer>
    requires Container<TContainer, typename TContainer::value_type>
    [[nodiscard]] auto store(TContainer& data_storage) {
        using DataType = TContainer::value_type;

        auto lambda_strategy = [&data_storage](DataType data) {
            if constexpr (requires { data_storage.push_back(std::move(data)); }) {
                data_storage.push_back(std::move(data));
            } else if constexpr (requires { data_storage.push(std::move(data)); }) {
                data_storage.push(std::move(data));
            }
        };

        return core::make_consumer(lambda_strategy);
    }

    template<typename T = std::string>
    [[nodiscard]] auto logInfo() {
        return core::make_consumer([](T data) {
            utils::Logger::log(utils::LogLevel::INFO, "{}", data);
        });
    }

    template<typename T = std::string>
    [[nodiscard]] auto logWarn() {
        return core::make_consumer([](T data) {
            utils::Logger::log(utils::LogLevel::WARN, "{}", data);
        });
    }

    template<typename T = std::string>
    [[nodiscard]] auto logError() {
        return core::make_consumer([](T data) {
            utils::Logger::log(utils::LogLevel::ERROR, "{}", data);
        });
    }
}
