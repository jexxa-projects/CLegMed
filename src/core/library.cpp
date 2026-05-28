#include "../../include/clegmed/core/library.h"

#include <iostream>

namespace clegmed::core {
    std::string hello() {
        std::cout << "Hello, World!" << std::endl;
        return "Hello World";
    }
}