#pragma once

#include <map>
#include <string>
#include <any>
#include <typeindex>
namespace clegmed::utils {

    class PropertiesLoader {
        inline static std::string default_config_file = "clegmed.toml";
        std::map<std::type_index, std::any> m_registry;

    public:




    };

}
