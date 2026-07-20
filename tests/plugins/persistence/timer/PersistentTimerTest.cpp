#include "clegmed/plugins/persistence/repository/imdb/IMDBRepository.hpp"
#include "clegmed/plugins/persistence/timer/PersistentTimer.hpp"
#include "clegmed/plugins/persistence/timer/TimerUtils.hpp"
#include "clegmed/plugins/shortcuts.hpp"
#include "../../../utils/Await.hpp"

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
}

TEST(PersistentTimerTest, PersistentTimerGraph) {
    //Arrange
    auto interval = 2ms;

    auto repository = IMDBRepository<TimerState>();
    auto timer_config = TimerConfig::timerConfigOf(TimerId{"TestTimer"});
    auto object_under_test = PersistentTimer(timer_config, repository);

    auto result = std::vector<TimeInterval>{};
    auto consumer = [&interval, &result] (const TimeInterval& time_interval) {
        // We check if that the interval is at least 'interval'. Note! the first time_interval could have smaller
        // interval because the flow graph immediately starts and does not wait for 'interval' for the first run
        if (time_interval.end - time_interval.begin >= interval) {
            result.push_back(time_interval);
        }
    };

    auto clegmed = CLegMed(
        FlowGraph{}
        .every(interval)
        .from(std::move(object_under_test))
        .consumeWith(consumer)
    );

    //Act
    clegmed.start();

    //Assert
    EXPECT_TRUE(await_condition(interval * 15, [&result]  {return result.size() > 10;}));
    clegmed.stop();
}
