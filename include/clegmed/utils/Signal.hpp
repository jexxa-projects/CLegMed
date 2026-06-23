#pragma once

#include <csignal>
#include <string_view>

namespace clegmed::utils {
    [[nodiscard]] constexpr std::string_view safe_signal_string(const int sig) noexcept {
        switch (sig) {
            case SIGINT:  return "SIGINT (Interrupt)";
            case SIGTERM: return "SIGTERM (Termination)";
            case SIGABRT: return "SIGABRT (Abort)";
            case SIGSEGV: return "SIGSEGV (Segmentation Fault)";
            case SIGFPE:  return "SIGFPE (Floating Point Exception)";
            case SIGILL:  return "SIGILL (Illegal Instruction)";
#if defined(__linux__) || defined(__APPLE__)
            case SIGHUP:  return "SIGHUP (Hangup)";
            case SIGKILL: return "SIGKILL (Kill)";
#endif
            default:      return "Unknown Signal";
        }
    }
}