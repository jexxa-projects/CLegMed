#pragma once

#include <string>
#include <cstdint>
#include "../../../include/clegmed/utils/Properties.hpp"

namespace clegmed::utils {
    class Properties;

    // Unter-Strukturen für die einzelnen Sektionen -> Using free function
    struct StringProperties {
        std::string normal;
        std::string literal;
        std::string multi_normal;
    };

    [[maybe_unused]]
    static void fromProperties(const Properties& properties, StringProperties& stringProperties) {
        stringProperties.normal = properties.get<std::string>("normal");
        stringProperties.literal = properties.get<std::string>("literal");
        stringProperties.multi_normal = properties.get<std::string>("multi_normal");
    }

    //For this struct we use static method
    struct IntegerProperties {
        int32_t positive{0};
        int32_t negative{0};
        int32_t decimal{0};
        int32_t with_underscores{0};
        int32_t binary{0};
        int32_t octal{0};
        u_int32_t hexadecimal{0};
        [[maybe_unused]]
        static void fromProperties(const Properties& properties, IntegerProperties& integer_properties) {
            integer_properties.positive = properties.get<int32_t>("positive");
            integer_properties.negative = properties.get<int32_t>("negative");
            integer_properties.decimal  = properties.get<int32_t>("decimal");
            integer_properties.with_underscores = properties.get<int32_t>("with_underscores");
            integer_properties.binary = properties.get<int32_t>("binary");
            integer_properties.octal = properties.get<int32_t>("octal");
            integer_properties.hexadecimal = properties.get<u_int32_t>("hexadecimal");
        }
    };

    // Here we use public method from instance
    struct ServerInfo {
        std::string ip;
        std::string role;

        [[maybe_unused]]
        void fromProperties(const Properties& properties) {
            ip = properties.get<std::string>("ip");
            role = properties.get<std::string>("role");
        }
    };

    struct ServersProperties {
        ServerInfo alpha;
        ServerInfo beta;
    };
    [[maybe_unused]]
    static void fromProperties(const Properties& properties, ServersProperties& servers_properties) {
        servers_properties.alpha.fromProperties(properties.subProperties("alpha").value());
        servers_properties.beta.fromProperties(properties.subProperties("beta").value());
    }

    // Das Haupt-Struct, das alles bündelt
    struct AppProperties {
        StringProperties strings;
        IntegerProperties integers;
        ServersProperties servers;
    };

    [[maybe_unused]]
    static void fromProperties(const Properties& properties, AppProperties& app_properties) {
        fromProperties(properties, app_properties.strings);
        IntegerProperties::fromProperties(properties, app_properties.integers);
        fromProperties(properties, app_properties.servers);
    }

    struct InvalidPropertiesStruct { //Does not provide any fromProperties method
        std::string ip;
        std::string role;
    };

} // namespace clegmed::utils