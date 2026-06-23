#pragma once
#include "Filter.hpp"
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
            // Speichert die Verbindung dauerhaft im Member
            m_input_pipe = std::forward<InputPipeType>(input_pipe);
        }


        void forward(T&& data) noexcept {
            if (m_input_pipe) { // Prüft zur Laufzeit, ob verbunden
                m_input_pipe(std::move(data));
            } else {
                std::cerr << "OutputPipe is not connected -> Discard data  " << data << std::endl;
            }
        }

    private:
        std::function<void(T)> m_input_pipe;
        const Filter& m_filter;
    };
}