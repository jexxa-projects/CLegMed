#pragma once
#include <utility>

namespace clegmed::core {

    template<typename TLeft, typename TRight>
    requires requires(TLeft& l, TRight& r) {
        l.outputPipe(); // Prüft, ob links Senden unterstützt
        r.inputPipe();  // Prüft, ob rechts Empfangen unterstützt
    }
    // TLeft&& und TRight&& akzeptieren Lvalues UND temporäre Rvalues
    decltype(auto) operator|(TLeft&& left, TRight&& right) {

        // Die Verbindung herstellen
        left.outputPipe().connect(right.inputPipe());

        // WICHTIG: Da right als Rvalue reinkommen kann,
        // müssen wir es per std::move weiterwerfen, statt es zu kopieren!
        return right;
    }
} // namespace clegmed::core
