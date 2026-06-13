//
// Created by Michael Repplinger on 10.06.26.
//
#include <string>

#include "clegmed/plugins/generic/GenericConsumer.hpp"
#include "gtest/gtest.h"


TEST(GenericPluginsTest, StoreConsumer) {
    // Arrange
    using namespace clegmed::plugins::generic;
    constexpr std::string message = "Hello World";
    std::vector<std::string> data_storage;

    auto object_under_test = store(data_storage);

    // Act: Produce data
    object_under_test.inputPipe()(message);

    // Assert: Validate if passed
    EXPECT_EQ(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], message);
}

TEST(GenericPluginsTest, DiscardConsumer) {
    // Arrange
    using namespace clegmed::plugins::generic;
    constexpr std::string message = "Hello World";
    std::vector<std::string> data_storage;

    auto object_under_test = discard<std::string>();

    // Act: Produce data
    object_under_test.inputPipe()(message);

    // Assert: Validate if passed
    EXPECT_TRUE(data_storage.empty());
}
