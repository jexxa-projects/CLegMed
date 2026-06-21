# Enable CMake's internal testing infrastructure
include(CTest)
include(GoogleTest)

# Automatically download and include GoogleTest from GitHub
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.17.0 # Secure, reproducible release tag for production
)

# Forbit gtest from overriding our global compiler settings settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# --- 7.2 GOOGLE BENCHMARK (Für Performance-Messungen) ---
FetchContent_Declare(
        google_benchmark
        GIT_REPOSITORY https://github.com/google/benchmark
        GIT_TAG        v1.9.5
)
# Interne Tests von Google Benchmark deaktivieren, um Build-Zeit zu sparen
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(google_benchmark)

add_subdirectory(core)
add_subdirectory(plugins/generic)
add_subdirectory(utils)