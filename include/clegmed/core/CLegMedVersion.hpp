#pragma once
#include <array>
#include <string_view>

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
        // Berechnet die exakte Größe des benötigten Buffers zur Compilezeit
        constexpr size_t calculate_size() {
            // "X.Y.Z" benötigt 5 Zeichen + Status-Länge + 1 für Nullterminierung
            return 5 + CLEGMED_VERSION_STATUS.size() + 1;
        }

        // Struktur, die das std::array zur Compilezeit befüllt
        struct VersionStorage {
            std::array<char, calculate_size()> data{};

            constexpr VersionStorage() {
                size_t idx = 0;
                data[idx++] = static_cast<char>('0' + (CLEGMED_VERSION_MAJOR % 10));
                data[idx++] = '.';
                data[idx++] = static_cast<char>('0' + (CLEGMED_VERSION_MINOR % 10));
                data[idx++] = '.';
                data[idx++] = static_cast<char>('0' + (CLEGMED_VERSION_PATCH % 10));

                for (char c : CLEGMED_VERSION_STATUS) {
                    data[idx++] = c;
                }
                data[idx] = '\0'; // Nullterminierung für Kompatibilität mit const char*
            }
        };

        inline constexpr VersionStorage storage;
    }

    // Moderne Interfaces via std::string_view (Null-Overhead & absolut sicher)
    inline constexpr std::string_view CLEGMED_VERSION{detail::storage.data.data()};

    inline constexpr std::string_view APPLICATION_NAME{CLEGMED_APP_NAME};
    inline constexpr std::string_view APPLICATION_VERSION{CLEGMED_APP_VERSION};
}
