
#pragma once
#include <csignal>
#include <semaphore>

namespace clegmed::core {
    template<typename ... Args>
    class CLegMed;
}
extern "C" inline void handle_shutdown_signals(int signal);

namespace clegmed::core::detail {
    inline std::binary_semaphore& internal_semaphore() noexcept {
        static std::binary_semaphore semaphore{0};
        return semaphore;
    }

    inline std::atomic<int>& internal_atomic() noexcept {
        static std::atomic sig{0};
        return sig;
    }

    inline void set_received_signal(const int signal) noexcept {
        internal_atomic().store(signal);
        internal_semaphore().release();
    }

    inline int get_received_signal() noexcept {
        return internal_atomic().load();
    }

    inline void wait_for_signal() noexcept {
        internal_semaphore().acquire();
    }
}

// Signal handling
extern "C" inline void handle_shutdown_signals(const int signal) {
    if (signal == SIGINT || signal == SIGTERM || signal == SIGHUP) {
        clegmed::core::detail::set_received_signal(signal);
    }
}
