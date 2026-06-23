#include "clegmed/utils/Logger.hpp"
#include "gtest/gtest.h"

TEST(LoggerTest, LogInfo) {
    //Arrange
    using namespace clegmed::utils;

    //Act
    testing::internal::CaptureStdout();
    Logger::log(LogLevel::INFO, "Hello World!");
    std::string info_output = testing::internal::GetCapturedStdout();

    testing::internal::CaptureStdout();
    Logger::log(LogLevel::WARN, "Hello World!");
    std::string warn_output = testing::internal::GetCapturedStdout();

    testing::internal::CaptureStdout();
    Logger::log(LogLevel::ERROR, "Hello World!");
    std::string error_output = testing::internal::GetCapturedStdout();

    std::cout << info_output;
    std::cout << warn_output;
    std::cout << error_output;

    //Assert
    EXPECT_TRUE(info_output.find("[INFO]") != std::string::npos);
    EXPECT_TRUE(info_output.find("Hello World!") != std::string::npos);
    EXPECT_TRUE(info_output.find("\033[32m") != std::string::npos); // Green for INFO

    EXPECT_TRUE(warn_output.find("[WARN]") != std::string::npos);
    EXPECT_TRUE(warn_output.find("Hello World!") != std::string::npos);
    EXPECT_TRUE(warn_output.find("\033[33m") != std::string::npos); // Yellow for WARN

    EXPECT_TRUE(error_output.find("[ERROR]") != std::string::npos);
    EXPECT_TRUE(error_output.find("Hello World!") != std::string::npos);
    EXPECT_TRUE(error_output.find("\033[31m") != std::string::npos); // Red for ERROR
}
