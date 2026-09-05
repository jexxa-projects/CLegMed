
#pragma once
#include <string_view>
#include <clegmed/core/generated/CLegMedVersion.hpp>

namespace clegmed::core {
    struct VersionInfo {
        VersionInfo(std::string_view application) : application{ application } {}
        std::string_view application;
        std::string_view clegmed_version{ CLEGMED_VERSION   };
        std::string_view build_type{
            #ifdef NDEBUG
                "Release"
            #else
                "Debug"
            #endif
        };
    };
}