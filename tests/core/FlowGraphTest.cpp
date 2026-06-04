//
// Created by Michael Repplinger on 31.05.26.
//

#include "clegmed/core/Consumer.hpp"
#include "clegmed/core/Processor.hpp"
#include "clegmed/core/Producer.hpp"
#include "gtest/gtest.h"

TEST(FlowGraphTest, FlowGraphProcessesData) {
    //Arrange
    using namespace clegmed::core;

    const auto expected_result = "Hello World";
    std::vector<std::string> data_storage;

    auto producer_strategy = [] { return "Hello";};
    auto processor_strategy = [](const std::string &input){ return input + " World";};
    auto consumer_strategy = [&data_storage](const std::string &data) {data_storage.push_back(data);};

    auto producer = make_producer(producer_strategy);
    auto processor = make_processor(processor_strategy);
    auto consumer = make_consumer(consumer_strategy);

    //Act
    producer.outputPipe().connect(processor.inputPipe());
    processor.outputPipe().connect(consumer.inputPipe());
    producer.produce();

    //Assert
    EXPECT_EQ(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], expected_result);
}
