#pragma once
#include <utility>

#include "ProcessingError.hpp"

namespace clegmed::core {
    template <typename T>
    class PipelineError {
        public:
        explicit PipelineError(const T& data, ProcessingError processing_error)
            : m_data(data), m_processing_error(std::move(processing_error)) { }

        explicit PipelineError(ProcessingError processing_error)
            : m_data(nullptr), m_processing_error(std::move(processing_error)) { }

        [[nodiscard]]
        const T& get() const { return m_data; }

        [[nodiscard]]
        const ProcessingError& processingError() const { return m_processing_error; }
    private:
        std::unique_ptr<T> m_data;
        ProcessingError m_processing_error;
    };

}
