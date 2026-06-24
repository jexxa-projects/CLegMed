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



TEST(CLegMedTest, RunFlowGraphs) {
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
    auto m_thread_ptr = std::make_unique<std::jthread>
    ([&object_under_test] { object_under_test.run();});

    //Assert
    EXPECT_TRUE(await_condition(std::chrono::seconds(5), [&]{
       return data_storage_1.size()>= 10 && data_storage_2.size()>= 10;
    }));
    EXPECT_GE(data_storage_1.size(), 10);

    {
        SCOPED_TRACE("Send Shutdown-Signal to CLegMed");
        EXPECT_TRUE(m_thread_ptr->joinable());

        std::raise(SIGINT);
    }
    if (m_thread_ptr->joinable()) {
        m_thread_ptr->join();
    }
}

TEST(CLegMedTest, UseArgv) {
    //Arrange
    using namespace clegmed::core;
    std::vector<std::string> data_storage_1;
    std::array argv_modern = { const_cast<char*>("TestUseArgv") }; //NOSONAR

    auto flowgraph_1 = FlowGraph{}
    .every(std::chrono::milliseconds(10))
    .from([] { return "Hello";})
    .then([](const std::string &input){ return input + " World";})
    .consumeWith([&data_storage_1](const std::string &data) {data_storage_1.push_back(data);});

    auto object_under_test = CLegMed(1, argv_modern.data(), std::move(flowgraph_1));

    //Act
    auto m_thread_ptr = std::make_unique<std::jthread>
    ([&object_under_test] { object_under_test.run();});

    //Assert
    EXPECT_TRUE(await_condition(std::chrono::seconds(5), [&]{
       return data_storage_1.size()>= 10;
    }));
    EXPECT_GE(data_storage_1.size(), 10);

    {
        SCOPED_TRACE("Send Shutdown-Signal to CLegMed");
        EXPECT_TRUE(m_thread_ptr->joinable());

        std::raise(SIGINT);
    }
    if (m_thread_ptr->joinable()) {
        m_thread_ptr->join();
    }
}

