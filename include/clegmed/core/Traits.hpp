#pragma once
#include <type_traits>
#include <tuple>

// Vorwärtsdeklaration, damit die Traits wissen, was eine OutputPipe ist
namespace clegmed::core {
    template <typename T>
    class OutputPipe;
}

namespace clegmed::core::detail {

    // =========================================================================
    // 1. Function Traits (Analysiert Lambda-Signaturen)
    // =========================================================================
    template <typename T>
    struct function_traits : function_traits<decltype(&T::operator())> {};

    template <typename ClassType, typename ReturnType, bool IsNoexcept, typename... Args>
    struct function_traits<ReturnType(ClassType::*)(Args...) const noexcept(IsNoexcept)> {
        using result_type = std::remove_cvref_t<ReturnType>;

        // extract noexcept
        static constexpr bool is_noexcept = IsNoexcept;

        // Indizierter Zugriff auf die Argumente (0, 1, 2...)
        template <std::size_t Index>
        using argument_t = std::decay_t<std::tuple_element_t<Index, std::tuple<Args...>>>;
    };

    // =========================================================================
    // 2. Universeller Template Type Extractor
    // =========================================================================
    // Grundstruktur
    template <typename T>
    struct extract_pipe_type;

    template <template <typename...> class TemplateClass, typename T, typename... Rest>
    struct extract_pipe_type<TemplateClass<T, Rest...>> {
        using type = T;
    };

    // Komfort-Alias bleibt identisch
    template <typename T>
    using extract_pipe_type_t = extract_pipe_type<T>::type;

} // namespace clegmed::core::detail

