#include <string>

#include "clegmed/plugins/shortcuts.hpp"
#include "clegmed/plugins/generic/GenericConsumer.hpp"
#include "gtest/gtest.h"


TEST(GenericPluginsTest, StoreConsumer) {
    // Arrange
    using namespace clegmed::shortcuts;
    const auto message = "Hello World";
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
    using namespace clegmed::shortcuts;
    const auto message = "Hello World";

    auto object_under_test = discard<std::string>();

    // Act/Assert: Produce data
    object_under_test.inputPipe()(message);
}


TEST(GenericPluginsTest, LogInfoConsumer) {
    // Arrange
    using namespace clegmed::shortcuts;
    const auto message = "Hello World";

    testing::internal::CaptureStdout();
    auto object_under_test = logInfo<std::string>();

    // Act/Assert: Produce data
    object_under_test.inputPipe()(message);
    const std::string info_output = testing::internal::GetCapturedStdout();

    //Assert
    EXPECT_TRUE(info_output.find("[INFO]") != std::string::npos);
}
