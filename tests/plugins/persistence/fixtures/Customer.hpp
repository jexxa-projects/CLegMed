
#pragma once
#include <string>

namespace clegmed::plugins::persistence::fixtures {
    class Customer {
        int m_id;
        std::string m_name;
    public:
        Customer(const int id, std::string name) : m_id(id), m_name(std::move(name)) {}
        Customer(const Customer&) = delete;
        Customer(Customer&&) = default;

        void setName(std::string name)            { m_name = std::move(name); }
        [[nodiscard]] std::string getName() const { return m_name; }
        [[nodiscard]] int aggregateId()     const { return m_id; }
    };
}
