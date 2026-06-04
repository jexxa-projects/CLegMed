//
// Created by Michael Repplinger on 31.05.26.
//

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
    struct function_traits;

    // Spezialisierung für const Member-Funktionen (Standard bei Lambdas)
    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits<ReturnType(ClassType::*)(Args...) const> {

        using result_type = std::remove_cvref_t<ReturnType>;

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
    using extract_pipe_type_t = typename extract_pipe_type<T>::type;

} // namespace clegmed::core::detail

