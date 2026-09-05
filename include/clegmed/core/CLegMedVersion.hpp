#pragma once
#define CLEGMED_VERSION_MAJOR 0
#define CLEGMED_VERSION_MINOR 1
#define CLEGMED_VERSION_PATCH 9
#define CLEGMED_VERSION_STATUS "-SNAPSHOT" // Für Releases einfach auf "" setzen

namespace clegmed::core {

    #define CLEGMED_STR_HELPER(x) #x
    #define CLEGMED_STR(x) CLEGMED_STR_HELPER(x)

    inline constexpr const char* CLEGMED_VERSION = CLEGMED_STR(CLEGMED_VERSION_MAJOR) "."
                                                   CLEGMED_STR(CLEGMED_VERSION_MINOR) "."
                                                   CLEGMED_STR(CLEGMED_VERSION_PATCH)
                                                   CLEGMED_VERSION_STATUS;

    #undef CLEGMED_STR
    #undef CLEGMED_STR_HELPER

}