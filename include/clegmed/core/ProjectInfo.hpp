
#pragma once
#include <string_view>
#include <clegmed/core/generated/Version.hpp>

namespace clegmed::core {
    struct ProjectInfo {
        std::string_view library_version{ CLEGMED_VERSION   };
        std::string_view build_type{
            #ifdef NDEBUG
                "Release"
            #else
                "Debug"
            #endif
        };
    };
}