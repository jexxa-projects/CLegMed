#pragma once
#include <iostream>
#include <chrono>
#include <format>
#include <string_view>

namespace clegmed::utils {
    enum class LogLevel {
        INFO,
        WARN,
        ERROR
    };

    class Logger {
        static constexpr std::string_view RESET = "\o{33}[0m";
        static constexpr std::string_view GREEN = "\o{33}[32m";
        static constexpr std::string_view YELLOW = "\o{33}[33m";
        static constexpr std::string_view RED = "\o{33}[31m";

        static std::string get_timestamp() {
            auto now = std::chrono::system_clock::now();
            return std::format("{:%FT%T%z}", now);
        }

        static constexpr std::string_view level_string(const LogLevel level) {
            switch (level) {
                    using enum LogLevel;
                case INFO: return "[INFO]";
                case WARN: return "[WARN]";
                case ERROR: return "[ERROR]";
            }
            return "UNKNOWN";
        }

        static constexpr std::string_view level_color(const LogLevel level) {
            switch (level) {
                using enum LogLevel;
                case INFO: return GREEN;
                case WARN: return YELLOW;
                case ERROR: return RED;
            }
            return RESET;
        }

    public:
        template<typename... Args>
        static void log(const LogLevel level, std::format_string<Args...> fmt, Args &&... args) {
            std::string message = std::format(fmt, std::forward<Args>(args)...);

            std::string timestamp = get_timestamp();
            auto level_str = level_string(level);
            auto color = level_color(level);

            std::cout << std::format("[{}] {}{:<7}{} {}\n",
                                     timestamp, color, level_str, RESET, message);
        }
    };
}
