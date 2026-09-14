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
    auto result = AppProperties();

    //Act
    auto properties = Properties::fromFile("clegmed.toml").value();
    result.strings  = properties.get<StringProperties>("strings");
    result.integers = properties.get<IntegerProperties>("integers");
    result.servers  = properties.get<ServersProperties>("servers");

    //Assert
    ASSERT_FALSE(result.strings.literal.empty());
    ASSERT_FALSE(result.strings.multi_normal.empty());
    ASSERT_FALSE(result.strings.normal.empty());
}
TEST(PropertiesConceptTest, InvalidStructFailsConcept) {
    using namespace clegmed::utils;
    static_assert(! DeserializableFromProperties<InvalidPropertiesStruct>,
                  "InvalidPropertiesStruct must not fulfill the concept!");
}