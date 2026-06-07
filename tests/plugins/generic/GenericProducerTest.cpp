//
// Created by Michael Repplinger on 07.06.26.
//

#include <string>

#include "clegmed/plugins/generic/GenericProducer.hpp"
#include "gtest/gtest.h"

TEST(GenericPluginsTest, EmitProducer) {
    // Arrange
    using namespace clegmed::plugins::generic;
    constexpr std::string message = "Hello World";
    std::vector<std::string> data_storage;

    auto object_under_test = emit(message);

    object_under_test.outputPipe().connect([&data_storage](std::string data) {
        data_storage.push_back(std::move(data));
    });

    // Act: Produce data
    object_under_test.produce();

    // Assert: Validate if passed
    EXPECT_EQ(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], message);
}

