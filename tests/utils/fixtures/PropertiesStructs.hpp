#pragma once

#include <string>
#include <vector>

#include "../../../include/clegmed/utils/Properties.hpp"

namespace clegmed::utils {
    class Properties;

    // Unter-Strukturen für die einzelnen Sektionen
    struct StringProperties {
        std::string normal;
        std::string literal;
        std::string multi_normal;
    };

    static void fromProperties(const Properties& properties, StringProperties& stringProperties) {
        stringProperties.normal = properties.get<std::string>("normal");
        stringProperties.literal = properties.get<std::string>("literal");
        stringProperties.multi_normal = properties.get<std::string>("multi_normal");

    }

    struct IntegerProperties {
        int positive{0};
        int negative{0};
        int decimal{0};
        long with_underscores{0};
        int binary{0};
        int octal{0};
        unsigned int hexadecimal{0};
    };

    struct ServerInfo {
        std::string ip;
        std::string role;
    };

    struct ServersProperties {
        ServerInfo alpha;
        ServerInfo beta;
    };

    // Das Haupt-Struct, das alles bündelt
    struct AppProperties {
        StringProperties strings;
        IntegerProperties integers;
        ServersProperties servers;
    };

} // namespace clegmed::utils