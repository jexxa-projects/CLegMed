#include "clegmed/plugins/shortcuts.hpp"
#include "clegmed/plugins/persistence/repository/Repository.hpp"
#include "clegmed/plugins/persistence/repository/imdb/IMDBRepository.hpp"
#include "gtest/gtest.h"


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

using namespace clegmed::plugins::persistence;

static_assert(isRepository<IMDBRepository<Customer>, Customer>,
    "InMemoryRepository must fulfill the isRepository concept!");



TEST(IMDBRepositoryTest, StoreCustomer) {
    // Arrange
    auto object_under_test = clegmed::plugins::persistence::IMDBRepository<Customer>();
    constexpr auto customer_count = 10;
    constexpr auto customer_name = "HelloWorld";

    std::ranges::for_each(std::views::iota(0, customer_count), [&](const int i) {
        object_under_test.add(Customer(i, customer_name));
    });

    // Act
    const auto customer_handle = object_under_test.get(1);

    // Assert: Validate if passed
    EXPECT_EQ(true, customer_handle.has_value());
    EXPECT_EQ(customer_name, customer_handle->getName());
    EXPECT_EQ(1, customer_handle->aggregateId());
    EXPECT_EQ(customer_count, object_under_test.getAll().size());
}


TEST(IMDBRepositoryTest, AddCustomerTwice) {
    // Arrange
    auto object_under_test = clegmed::plugins::persistence::IMDBRepository<Customer>();
    constexpr auto customer_id = 1;
    constexpr auto customer_name = "HelloWorld";
    object_under_test.add(Customer(customer_id, customer_name));

    // Act/Assert
    EXPECT_THROW(object_under_test.add(Customer(customer_id, customer_name)), std::invalid_argument);
}

TEST(IMDBRepositoryTest, ClearCustomer) {
    // Arrange
    auto object_under_test = clegmed::plugins::persistence::IMDBRepository<Customer>();
    constexpr auto customer_count = 10;
    const auto customer_name = "HelloWorld";

    std::ranges::for_each(std::views::iota(0, customer_count), [&](const int i) {
        object_under_test.add(Customer(i, customer_name));
    });

    // Act
    const auto result = object_under_test.getAll().size();
    object_under_test.removeAll();

    // Assert: Validate if passed
    EXPECT_EQ(customer_count, result);
    EXPECT_TRUE(object_under_test.getAll().empty());
}

TEST(IMDBRepositoryTest, UpdateCustomer) {
    // Arrange
    auto object_under_test = clegmed::plugins::persistence::IMDBRepository<Customer>();
    const auto customer_id   = 1;
    const auto customer_old_name = "OldName";
    const auto customer_new_name = "NewName";

    object_under_test.add(Customer(customer_id, customer_old_name));

    // Act
    auto& result = object_under_test.get(customer_id).or_else_throw();
    result.setName(customer_new_name);
    object_under_test.update(result);

    // Assert: Validate if passed
    EXPECT_EQ(customer_new_name, object_under_test
        .get(customer_id)
        .or_else_throw()
        .getName()
        );
}