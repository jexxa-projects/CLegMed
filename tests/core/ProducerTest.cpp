#include "clegmed/core/Processor.hpp"
#include "clegmed/core/Producer.hpp"
#include "../utils/fixtures/PropertiesStructs.hpp"
#include "gtest/gtest.h"

TEST(CoreTest, ProducerSendsData) {
    // Arrange
    using namespace clegmed::core;

    constexpr auto expected_result = "Hello World";
    std::vector<std::string> data_storage;
    constexpr auto test_strategy = [] { return "Hello World";};

    const auto object_under_test = make_producer(test_strategy);

    object_under_test->outputPipe().connect([&data_storage](std::string data) {
        data_storage.push_back(std::move(data));
    });

    // Act: Put some data into the input pipe
    object_under_test->produce();

    // Assert: Validate if passed
    EXPECT_EQ(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], expected_result);
}

TEST(CoreTest, ProducerSendsDataByOutputPipe) {
    // Arrange
    using namespace clegmed::core;

    constexpr auto expected_result = "Hello World";
    std::vector<std::string> data_storage;
    constexpr auto test_strategy = [](auto& outputPipe) { outputPipe.forward( "Hello World");};

    const auto object_under_test = make_piped_producer<std::string>(test_strategy);

    object_under_test->outputPipe().connect([&data_storage](std::string data) {
        data_storage.push_back(std::move(data));
    });

    // Act: Put some data into the input pipe
    object_under_test->produce();

    // Assert: Validate if passed
    EXPECT_EQ(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], expected_result);
}


TEST(CoreTest, MakeConfiguredProducer) {
    // Arrange
    using namespace clegmed::core;

    const auto properties = clegmed::utils::Properties::fromFile("filterproperties.toml").value();
    const auto [ip, port] = properties.get<clegmed::utils::ServerInfo>("processor");
    const auto expected_result = "Hello World to " + ip;

    std::vector<std::string> data_storage;
    constexpr auto test_strategy = [](
        const clegmed::utils::ServerInfo& server_info) {
        return "Hello World to " + server_info.ip;
    };

    const auto object_under_test = make_configured_producer(test_strategy);
    object_under_test->withProperties("processor");
    object_under_test->properties(properties);

    object_under_test->outputPipe().connect([&data_storage](std::string data) {
        data_storage.push_back(std::move(data));
    });

    // Act: Put some data into the input pipe
    object_under_test->produce();

    // Assert: Validate if passed
    EXPECT_EQ(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], expected_result);
}