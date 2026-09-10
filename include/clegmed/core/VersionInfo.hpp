
#pragma once
#include <string_view>
#include "CLegMedVersion.hpp"

namespace clegmed::core {
    struct VersionInfo {
        VersionInfo(std::string application_name) :
            m_application_name{ std::move(application_name) } {}
        VersionInfo() :
            m_application_name{ APPLICATION_NAME } {}
        std::string m_application_name;
        std::string_view m_application_version{ APPLICATION_VERSION};
        std::string_view m_clegmed_version{ CLEGMED_VERSION   };
        std::string_view m_build_type{
            #ifdef NDEBUG
                "Release"
            #else
                "Debug"
            #endif
        };
    };
}