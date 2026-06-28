#include <string>

#include "clegmed/plugins/generic/GenericProducer.hpp"
#include "gtest/gtest.h"
#include "clegmed/plugins/shortcuts.hpp"

TEST(GenericPluginsTest, EmitProducer) {
    // Arrange
    using namespace clegmed::shortcuts;
    const auto message = "Hello World";
    std::vector<std::string> data_storage;

    auto object_under_test = emit(message);

    object_under_test.outputPipe().connect([&data_storage](std::string data) {
        data_storage.push_back(std::move(data));
    });

    // Act: Produce data
    object_under_test.produce();
    object_under_test.produce();

    // Assert: Validate if passed
    EXPECT_EQ(data_storage.size(), 2);
    EXPECT_EQ(data_storage[0], message);
    EXPECT_EQ(data_storage[1], message);
}

