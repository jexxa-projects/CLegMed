#pragma once
#include <chrono>
#include <thread>

bool await_condition(const std::chrono::milliseconds timeout, auto condition) {
    const auto start_zeit = std::chrono::steady_clock::now();

    while (true) {
        if (condition()) {
            return true;
        }

        if (std::chrono::steady_clock::now() - start_zeit >= timeout) {
            return false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}
