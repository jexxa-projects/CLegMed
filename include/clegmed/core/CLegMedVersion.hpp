#pragma once
#include <array>
#include <string_view>

// 1. Typsichere constexpr-Konstanten statt #define (beseitigt die Warnung)
inline constexpr int CLEGMED_VERSION_MAJOR = 0;
inline constexpr int CLEGMED_VERSION_MINOR = 1;
inline constexpr int CLEGMED_VERSION_PATCH = 9;
inline constexpr std::string_view CLEGMED_VERSION_STATUS = "-SNAPSHOT";

#ifndef CLEGMED_APP_NAME
#define CLEGMED_APP_NAME "Unknown Application"
#endif

#ifndef CLEGMED_APP_VERSION
#define CLEGMED_APP_VERSION "0.0.0-SNAPSHOT"
#endif

namespace clegmed::core {

    namespace detail {
        // Hilfsfunktion zur Ermittlung der exakten Buffer-Größe zur Compilezeit
        constexpr size_t calculate_version_buffer_size() {
            // "X.Y.Z" benötigt mindestens 5 Zeichen + Status-Länge + 1 für die Nullterminierung '\0'
            return 5 + CLEGMED_VERSION_STATUS.size() + 1;
        }

        // Struktur baut den String zur Compilezeit zeichenweise zusammen
        struct CompileTimeVersion {
            std::array<char, calculate_version_buffer_size()> data{};

            constexpr CompileTimeVersion() {
                size_t idx = 0;
                data[idx++] = static_cast<char>('0' + CLEGMED_VERSION_MAJOR % 10);
                data[idx++] = '.';
                data[idx++] = static_cast<char>('0' + CLEGMED_VERSION_MINOR % 10);
                data[idx++] = '.';
                data[idx++] = static_cast<char>('0' + CLEGMED_VERSION_PATCH % 10);

                for (const char c : CLEGMED_VERSION_STATUS) {
                    data[idx++] = c;
                }
                data[idx] = '\0'; // Wichtig für die Kompatibilität mit C-Strings (const char*)
            }
        };

        // Instanziierung zur Kompilierzeit
        inline constexpr CompileTimeVersion version_storage;
    }

    // Vollständig kompatible Interfaces (Zuweisung an const char* funktioniert wie gewohnt)
    inline constexpr const char* CLEGMED_VERSION = detail::version_storage.data.data();

    inline constexpr auto APPLICATION_NAME = CLEGMED_APP_NAME;
    inline constexpr auto APPLICATION_VERSION = CLEGMED_APP_VERSION;
}
