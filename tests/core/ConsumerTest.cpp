#include "../utils/fixtures/PropertiesStructs.hpp"
#include "clegmed/plugins/shortcuts.hpp"
#include "gtest/gtest.h"

namespace clegmed::utils {
    struct ServerInfo;
}

TEST(CoreTest, MakeConsumer) {
    // Arrange
    using namespace clegmed::shortcuts;
    using namespace std::string_view_literals;
    constexpr auto expected_result = "Hello World";

    std::vector<std::string> data_storage;
    auto test_strategy = [&data_storage](std::string data) {
        data_storage.push_back(std::move(data));
    };


    const auto object_under_test = make_consumer(test_strategy);

    // Act: Put some data into the input pipe
    object_under_test->inputPipe()(expected_result);

    // Assert: Validate if passed
    EXPECT_EQ(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], expected_result);
}



TEST(CoreTest, MakeConfiguredConsumer) {
    // Arrange
    using namespace clegmed::shortcuts;
    using namespace std::string_view_literals;
    constexpr auto message = "Hello World to ";
    const auto properties = Properties::fromFile("filterproperties.toml").value();
    const auto [ip, port] = properties.get<clegmed::utils::ServerInfo>("processor");
    const auto expected_result = message + ip;


    std::vector<std::string> data_storage;
    auto test_strategy = [&data_storage](const std::string& data, const clegmed::utils::ServerInfo& server) {
        data_storage.push_back(data + server.ip);
    };

    const auto object_under_test = make_configured_consumer(test_strategy);
    object_under_test->withProperties("processor");
    object_under_test->properties(properties);


    // Act: Put some data into the input pipe
    object_under_test->inputPipe()(message);

    // Assert: Validate if passed
    EXPECT_EQ(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], expected_result);
}
