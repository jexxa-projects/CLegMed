
#include "../../../utils/Await.hpp"
#include "gtest/gtest.h"
#include "clegmed/plugins/shortcuts.hpp"
#include "../fixtures/Customer.hpp"
#include "clegmed/plugins/persistence/generic/PersistentConsumer.hpp"
#include "clegmed/plugins/persistence/repository/imdb/IMDBRepository.hpp"


using namespace clegmed::plugins::persistence;
using namespace clegmed::plugins::persistence::fixtures;



TEST(PersistentConsumerTest, PersistCustomer) {
    // Arrange
    constexpr auto customer_name = "HelloWorld";
    auto repository = IMDBRepository<Customer>{};
    repository.init();

    auto object_under_test = persistentStore(repository);

    // Act
    object_under_test.consume(Customer(1, customer_name));

    // Assert: Validate if passed
    EXPECT_EQ(1, repository.getAll().size());
}

TEST(PersistentConsumerTest, PersistCustomerFlowGraph) {
    // Arrange
    using namespace clegmed::shortcuts;
    auto repository = IMDBRepository<Customer>{};
    repository.init();

    size_t max_count = 10;
    auto counter = std::atomic(0);
    auto producer = [&counter] { return Customer(++counter, "HelloWorld"); };

    auto clegmed = CLegMed(
        FlowGraph{}.repeat(max_count)
        .from(producer)
        .consumeWith(persistentStore(repository))
    );

    // Act
    clegmed.start();

    // Assert: Validate if passed
    await_condition(1s, [&repository, &max_count]{return repository.getAll().size() == max_count;});
    clegmed.stop();
}

