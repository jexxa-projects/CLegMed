#include <algorithm>
#include "clegmed/plugins/shortcuts.hpp"
#include "clegmed/plugins/persistence/timer/TimerUtils.hpp"
#include "gtest/gtest.h"

using namespace clegmed::shortcuts;
using namespace clegmed::plugins::persistence;




TEST(TimerUtilsTest, TimeInterval) {
    // Arrange
    const auto now = std::chrono::system_clock::now();
    const auto five_minutes_ago = now - 5min;

    auto [begin, end] = TimeInterval::of(five_minutes_ago, now);

    // Assert: Validate if passed
    EXPECT_EQ(5min, end - begin);
}
