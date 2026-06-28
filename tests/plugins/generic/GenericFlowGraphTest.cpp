#include "clegmed/core/Pipe.hpp"
#include "clegmed/core/Consumer.hpp"
#include "clegmed/core/Processor.hpp"
#include "clegmed/core/flowgraph/FlowGraph.hpp"
#include "clegmed/plugins/generic/GenericConsumer.hpp"
#include "clegmed/plugins/generic/GenericProcessor.hpp"
#include "clegmed/plugins/generic/GenericProducer.hpp"
#include "gtest/gtest.h"
#include "../../utils/Await.hpp"
#include "clegmed/plugins/shortcuts.hpp"

TEST(FlowGraphTest, GenericFlowGraphTest) {
    //Arrange
    using namespace clegmed::shortcuts;

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
    using namespace clegmed::shortcuts;

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


TEST(FlowGraphTest, RealFlowGraphTest) {
    //Arrange
    using namespace clegmed::shortcuts;

    const auto expected_result = "Hello World";
    std::vector<std::string> data_storage;
    //Act
    auto flowgraph = FlowGraph{}
        .every(std::chrono::milliseconds(10))
        .from(emit("Hello"))
        .then(append(" World"))
        .consumeWith(store(data_storage));

    //Act
    flowgraph.start();

    //Assert
    EXPECT_TRUE(await_condition(std::chrono::seconds(5), [&]{
       return !data_storage.empty();
    }));
    EXPECT_GE(data_storage.size(), 1);
    EXPECT_EQ(data_storage[0], expected_result);
    flowgraph.stop();
}



