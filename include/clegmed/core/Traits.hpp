//
// Created by Michael Repplinger on 31.05.26.
//

#pragma once
#include <type_traits>

//Deduction guide
namespace detail {
    template <typename T>
    struct function_traits;

    template <typename ClassType, typename ReturnType, typename Arg0>
    struct function_traits<ReturnType(ClassType::*)(Arg0) const> {
        using argument_type = std::decay_t<Arg0>;
        using result_type = std::decay_t<ReturnType>;
    };
}
