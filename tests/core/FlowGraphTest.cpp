//
// Created by Michael Repplinger on 31.05.26.
//

#include "clegmed/core/Processor.hpp"
#include "clegmed/core/Producer.hpp"
#include "gtest/gtest.h"

TEST(FlowGraphTest, FlowGraphProcessesData) {
    //Arrange
    const auto expected_result = "Hello World";
    std::vector<std::string> data_storage;
    auto producer_strategy = [] { return "Hello";};
    auto processor_strategy = [](const std::string &input){ return input + " World";};
    auto producer = clegmed::core::Producer<std::string, decltype(producer_strategy)>(producer_strategy);
    auto processor = clegmed::core::Processor<std::string, std::string, decltype(processor_strategy)>(processor_strategy);

    //Act
    producer.outputPipe().connect(processor.inputPipe());
    processor.outputPipe().connect([&data_storage](std::string data) {data_storage.push_back(std::move(data));});
    producer.produce();

    //Assert
    EXPECT_EQ(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], expected_result);
}
