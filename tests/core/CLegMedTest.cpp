//
// Created by Michael Repplinger on 20.06.26.
//
#include "clegmed/core/CLegMed.hpp"
#include "clegmed/core/flowgraph/FlowGraph.hpp"
#include "gtest/gtest.h"
#include "../utils/Await.hpp"

TEST(CLegMedTest, StartStopMultitpleFlowGraphs) {
    //Arrange
    using namespace clegmed::core;
    std::vector<std::string> data_storage_1;
    std::vector<std::string> data_storage_2;

    auto flowgraph_1 = FlowGraph{}
        .every(std::chrono::milliseconds(10))
        .from([] { return "Hello";})
        .then([](const std::string &input){ return input + " World";})
        .consumeWith([&data_storage_1](const std::string &data) {data_storage_1.push_back(data);});

    auto flowgraph_2 = FlowGraph{}
        .every(std::chrono::milliseconds(10))
        .from([] { return "Hello";})
        .then([](const std::string &input){ return input + " World";})
        .consumeWith([&data_storage_2](const std::string &data) { data_storage_2.push_back(data);});

    auto object_under_test = CLegMed(std::move(flowgraph_1), std::move(flowgraph_2));
    //Act
    object_under_test.start();

    //Assert
    EXPECT_TRUE(await_condition(std::chrono::seconds(5), [&]{
       return data_storage_1.size()>= 10 && data_storage_2.size()>= 10;
    }));
    EXPECT_GE(data_storage_1.size(), 10);
    object_under_test.stop();

}
