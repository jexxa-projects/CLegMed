#include <clegmed/core/library.h>
#include <iostream>
#include <format>  // C++20 Feature für saubere Textformatierung
#include <cassert> // Für die Test-Überprüfungen (Ähnlich wie JUnit Assertions)

int main() {
    std::cout << "[TEST] Starte Komponententest für clegmed::core::Engine...\n";

    // 1. Arrange: Objekt auf dem Stack instanziieren (Kein 'new' nötig!)

    // 2. Act: Die Methode deiner Shared Library aufrufen
    std::string result = hello();

    // 3. Saubere Log-Ausgabe mit C++20 std::format
    std::cout << std::format("[LOG] Erhaltene Rückgabe: '{}'\n", result);

    // 4. Assert: Überprüfen, ob das Ergebnis den Erwartungen entspricht
    // Wenn die Bedingung fehlschlägt, bricht das Programm sofort mit einer Fehlermeldung ab.
    assert(!result.empty());
    assert(result == "Hello World");

    std::cout << "[RESULT] Test erfolgreich bestanden! (Exit-Code 0)\n";
    return 0; // 0 signalisiert dem Betriebssystem (und CLion): Alles fehlerfrei!
}
