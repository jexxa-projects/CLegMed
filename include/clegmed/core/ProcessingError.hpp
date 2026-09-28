#pragma once
#include <stdexcept>

namespace clegmed::core {
    class ProcessingError {
    public:
        explicit ProcessingError(std::string filter, std::string error_message)
            : m_filter(std::move(filter)), m_error_message(std::move(error_message)){}

        [[nodiscard]] const std::string& filter() const { return m_filter; }
        [[nodiscard]] const std::string& errorMessage() const { return m_error_message; }
    private:
        std::string m_filter;
        std::string m_error_message;
    };
}
