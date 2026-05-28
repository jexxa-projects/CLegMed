cmake_minimum_required(VERSION 4.1)
project(clegmed VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(clegmed SHARED
        src/core/library.cpp
)

# Search path 
target_include_directories(clegmed PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)


# 2. Das Test-Programm definieren (eine normale Executable)
add_executable(hello_world_test
        tests/core/HelloWorld.cpp
)

#WICHTIG: Das Test-Programm mit deiner Shared Library verknüpfen (Linken)
target_link_libraries(hello_world_test PRIVATE clegmed)