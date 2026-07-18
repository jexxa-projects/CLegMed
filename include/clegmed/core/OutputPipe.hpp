#pragma once
#include "Filter.hpp"
#include "clegmed/utils/Logger.hpp"
#include <functional>
#include <utility>

namespace clegmed::core {
    template <typename T>
    class OutputPipe {
    public:
        OutputPipe() = delete;

        explicit OutputPipe(const Filter& filter) : m_filter(filter) {}
        virtual ~OutputPipe() = default;


        template <typename InputPipeType>
        void connect(InputPipeType&& input_pipe) {
            m_input_pipe = std::forward<InputPipeType>(input_pipe);
        }


        void forward(T&& data) noexcept {
            if (m_input_pipe) {
                m_input_pipe(std::move(data));
            } else {
                utils::Logger::log(
                    utils::LogLevel::ERROR,
                    "OutputPipe of filter '{}' is not connected -> Discard data",
                    m_filter.name()
                    );
            }
        }

    private:
        std::function<void(T)> m_input_pipe;
        const Filter& m_filter;
    };
}