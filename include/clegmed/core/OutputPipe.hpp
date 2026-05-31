//
// Created by Michael Repplinger on 29.05.26.
//

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


        void forward(T data) {
            if (m_input_pipe) { // Prüft zur Laufzeit, ob verbunden
                m_input_pipe(std::move(data));
            }
        }

    private:
        std::function<void(T)> m_input_pipe;
        const Filter& m_filter;
    };
}