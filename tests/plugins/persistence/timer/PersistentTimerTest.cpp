#include "clegmed/plugins/persistence/repository/imdb/IMDBRepository.hpp"
#include "clegmed/plugins/persistence/timer/PersistentTimer.hpp"
#include "clegmed/plugins/persistence/timer/TimerUtils.hpp"
#include "clegmed/plugins/shortcuts.hpp"
#include "gtest/gtest.h"
using namespace clegmed::shortcuts;
using namespace clegmed::plugins::persistence;




TEST(PersistentTimerTest, PersistentTimer) {
    //Arrange
    auto result = std::vector<TimeInterval>{};
    auto repository = IMDBRepository<TimerState>();
    auto timer_config = TimerConfig::timerConfigOf(TimerId{"TestTimer"});
    auto object_under_test = PersistentTimer(timer_config, repository);

    object_under_test.outputPipe().connect([&result](const TimeInterval time_interval){result.push_back(time_interval);});
    //Act
    object_under_test.produce();

    //Assert
    EXPECT_EQ(result.size(), 1);
    std::cout << result[0].begin << " - " << result[0].end << std::endl;
}
