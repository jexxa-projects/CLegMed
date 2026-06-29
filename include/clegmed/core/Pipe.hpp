#pragma once
#include <utility>

namespace clegmed::core {
    template<typename TLeft, typename TRight>
       concept ConnectablePipes = requires(TLeft& l, TRight& r) {
        l.outputPipe();
        r.inputPipe();
       };

    template<typename TLeft, typename TRight>
    requires ConnectablePipes<TLeft, TRight>
    decltype(auto) operator|(TLeft&& left, TRight&& right) { //NOSONAR - left is not used for forwarding
        left.outputPipe().connect(right.inputPipe());
        return std::forward<TRight>(right);
    }
}
