#pragma once

#include <string>
#include <filesystem>
#include <toml++/toml.hpp>

#include "Logger.hpp"

namespace clegmed::utils {

    class PropertiesLoader {
        inline static std::string default_config_file = "clegmed.toml";

        toml::table m_root_table;

    public:
        PropertiesLoader(std::string config_file) {
            if (std::filesystem::exists(config_file)) {
                try {
                    m_root_table = toml::parse_file(config_file);
                } catch (const toml::parse_error& err) {
                    Logger::log(LogLevel::ERROR, "TOML parse error: {}", err.description());
                }
            } else {
                Logger::log(LogLevel::ERROR, "TOML file '{}' does not exists!", config_file);
            }
        }




    };

}
