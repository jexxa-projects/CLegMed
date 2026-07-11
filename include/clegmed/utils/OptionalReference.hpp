
#pragma once

#include <optional>
#include <functional>
#include <type_traits>

namespace clegmed::utils {

// In case of normal values, directly delegate to std::optional
template <typename T>
class optional : public std::optional<T> {
    using std::optional<T>::optional;
};

// In case of references, provide our own optional. Note: This will become obsolete and can be removed in C++26
template <typename T>
class optional<T&> {
    T* _ptr = nullptr;

public:
    constexpr optional() noexcept = default;

    explicit constexpr optional(std::nullopt_t) noexcept {}
    explicit constexpr optional(T& reference) noexcept : _ptr(&reference) {}

    [[nodiscard]] constexpr bool has_value() const noexcept { return _ptr != nullptr; }
    [[nodiscard]] constexpr explicit operator bool() const noexcept { return _ptr != nullptr; }

    constexpr T* operator->() const noexcept { return _ptr; }
    constexpr T& operator*() const noexcept { return *_ptr; }

    constexpr T& value() const {
        if (!_ptr) throw std::bad_optional_access();
        return *_ptr;
    }

    constexpr T& value_or(T& fallback) const noexcept {
        return _ptr ? *_ptr : fallback;
    }

    template <typename F>
    constexpr auto and_then(F&& f) const {
        if (_ptr) {
            return std::invoke(std::forward<F>(f), *_ptr);
        }
        using ReturnType = std::invoke_result_t<F, T&>;
        return ReturnType{std::nullopt};
    }

    template <typename F>
    constexpr auto transform(F&& f) const {
        using RawResult = std::invoke_result_t<F, T&>;

        if (_ptr) {
            if constexpr (std::is_reference_v<RawResult>) {
                return optional<RawResult>(std::invoke(std::forward<F>(f), *_ptr));
            } else {
                return std::optional<RawResult>(std::invoke(std::forward<F>(f), *_ptr));
            }
        }

        if constexpr (std::is_reference_v<RawResult>) {
            return optional<RawResult>{std::nullopt};
        } else {
            return std::optional<RawResult>{std::nullopt};
        }
    }

    template <typename F>
    constexpr optional or_else(F&& f) const {
        if (_ptr) return *this;
        return std::invoke(std::forward<F>(f));
    }
    constexpr T& or_else_throw() const {
        if (!_ptr) {
            throw std::bad_optional_access();
        }
        return *_ptr;
    }
};

} // namespace utils
