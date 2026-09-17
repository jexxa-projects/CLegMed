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
    const auto result = Properties::fromFile("clegmed.toml");

    //Assert
    EXPECT_TRUE(result.has_value());
}


TEST(PropertiesLoaderTest, ReadProperties) {
    //Arrange
    using namespace clegmed::utils;
    auto [strings, integers, servers] = AppProperties();

    //Act
    auto properties = Properties::fromFile("clegmed.toml").value();
    strings  = properties.get<StringProperties>("strings");
    integers = properties.get<IntegerProperties>("integers");
    servers  = properties.get<ServersProperties>("servers");

    //Assert
    ASSERT_FALSE(strings.literal.empty());
    ASSERT_FALSE(strings.multi_normal.empty());
    ASSERT_FALSE(strings.normal.empty());
}
TEST(PropertiesConceptTest, InvalidStructFailsConcept) {
    using namespace clegmed::utils;
    static_assert(! DeserializableFromProperties<InvalidPropertiesStruct>,
                  "InvalidPropertiesStruct must not fulfill the concept!");
}