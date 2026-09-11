#include "clegmed/utils/Properties.hpp"
#include "fixtures/PropertiesStructs.hpp"
#include "gtest/gtest.h"

namespace clegmed::utils {
    struct StringProperties;
}

TEST(PropertiesLoaderTest, FileNotFound) {
    //Arrange
    using namespace clegmed::utils;

    //Act
    auto result = Properties::fromFile("TEST.toml");

    //Assert
    EXPECT_EQ(result.error(), Properties::LoadError::FileNotFound);
}

TEST(PropertiesLoaderTest, ValidProperties) {
    //Arrange
    using namespace clegmed::utils;

    //Act
    auto result = Properties::fromFile("clegmed.toml");

    //Assert
    EXPECT_TRUE(result.has_value());
}


TEST(PropertiesLoaderTest, ReadProperties) {
    //Arrange
    using namespace clegmed::utils;

    //Act
    auto result =
        Properties::fromFile("clegmed.toml")->get<StringProperties>("strings");

    //Assert
    ASSERT_FALSE(result.literal.empty());
    ASSERT_FALSE(result.multi_normal.empty());
    ASSERT_FALSE(result.normal.empty());
}
