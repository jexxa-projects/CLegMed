#pragma once
#include <utility>

namespace clegmed::core {

    template<typename TLeft, typename TRight>
    requires requires(TLeft& l, TRight& r) {
        l.outputPipe();
        r.inputPipe();
    }
    decltype(auto) operator|(TLeft&& left, TRight&& right) {
        left.outputPipe().connect(right.inputPipe()); //NOSONAR - left is not used for forwarding
        return std::forward<TRight>(right);
    }
}
