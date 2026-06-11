//
// Created by Michael Repplinger on 31.05.26.
//

#include "clegmed/core/Pipe.hpp"
#include "clegmed/core/Consumer.hpp"
#include "clegmed/core/Processor.hpp"
#include "clegmed/plugins/generic/GenericConsumer.hpp"
#include "clegmed/plugins/generic/GenericProcessor.hpp"
#include "clegmed/plugins/generic/GenericProducer.hpp"
#include "gtest/gtest.h"

TEST(FlowGraphTest, GenericFlowGraphTest) {
    //Arrange
    using namespace clegmed::plugins::generic;

    const auto expected_result = "Hello World";
    std::vector<std::string> data_storage;

    auto producer = emit("Hello");
    auto processor = append(" World");
    auto consumer = store(data_storage);

    //Act
    producer.outputPipe().connect(processor.inputPipe());
    processor.outputPipe().connect(consumer.inputPipe());
    producer.produce();

    //Assert
    EXPECT_EQ(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], expected_result);
}


TEST(FlowGraphTest, PipeFlowGraphTest) {
    //Arrange
    using namespace clegmed::plugins::generic;

    const auto expected_result = "Hello World";
    std::vector<std::string> data_storage;

    auto producer = emit("Hello");
    auto processor = append(" World");
    auto consumer = store(data_storage);

    //Act
    producer | processor | consumer;

    //Act
    producer.produce();

    //Assert
    EXPECT_EQ(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], expected_result);
}
