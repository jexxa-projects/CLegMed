#include <string>

#include "clegmed/plugins/generic/GenericProcessor.hpp"
#include "clegmed/plugins/shortcuts.hpp"
#include "gtest/gtest.h"

TEST(GenericPluginsTest, PassThroughProcessor) {
    // Arrange
    using namespace clegmed::shortcuts;
    const auto message = "Hello World";
    std::vector<std::string> data_storage;

    auto object_under_test = passThrough().build<std::string>();

    object_under_test.outputPipe().connect([&data_storage](std::string data) {
        data_storage.push_back(std::move(data));
    });

    // Act: Produce data
    object_under_test.inputPipe()(message);

    // Assert: Validate if passed
    EXPECT_EQ(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], message);
}


TEST(GenericPluginsTest, TraceInfoPorcessor) {
    // Arrange
    using namespace clegmed::shortcuts;

    std::vector<std::string> data_storage;
    const auto message = "Hello World";

    testing::internal::CaptureStdout();
    auto object_under_test = traceInfo<std::string>();

    object_under_test.outputPipe().connect([&data_storage](std::string data) {
        data_storage.push_back(std::move(data));
    });

    // Act/Assert: Produce data
    object_under_test.inputPipe()(message);
    const std::string info_output = testing::internal::GetCapturedStdout();
    std::cout << info_output << std::endl;

    //Assert
    EXPECT_EQ(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], message);
    EXPECT_TRUE(info_output.find("[INFO]") != std::string::npos);
}
