#include "clegmed/plugins/shortcuts.hpp"
#include "clegmed/plugins/persistence/repository/Repository.hpp"
#include "clegmed/plugins/persistence/repository/imdb/IMDBRepository.hpp"
#include "gtest/gtest.h"


class Customer {
    int m_id;
    std::string m_name;
public:
    Customer(int id, std::string name) : m_id(id), m_name(std::move(name)) {}
    Customer(const Customer&) = delete;
    Customer(Customer&&) = default;

    int aggregateId() const { return m_id; }
    std::string getName() const { return m_name; }
};

using namespace clegmed::plugins::persistence;

// Prüft, ob InMemoryRepository<Customer> das isRepository-Concept für Customer erfüllt
static_assert(isRepository<IMDBRepository<Customer>, Customer>,
    "InMemoryRepository must fulfill the isRepository concept!");



TEST(IMDBRepositoryTest, StoreConsumer) {
    // Arrange
    auto repository = clegmed::plugins::persistence::IMDBRepository<Customer>();

    // Act
    repository.add(Customer(1, "Test"));

    auto customer_handle = repository.get(1);

    std::cout << repository.get(1)
                 .transform([](auto const& h) { return h->getName(); })
                 .value_or("Customer Not Found")
              << std::endl;

    // Assert: Validate if passed
    EXPECT_EQ(1, (*customer_handle)->aggregateId());
    EXPECT_EQ(1, repository.getAll().size());
}


