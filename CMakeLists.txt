# ============================================================================
# 1. Minimum + project + policies
# ============================================================================
cmake_minimum_required(VERSION 3.23)
cmake_policy(SET CMP0135 NEW)
cmake_policy(SET CMP0167 NEW)
project(project-euler-cpp LANGUAGES CXX)

# ============================================================================
# 2. Global language / standard settings
# ============================================================================
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================================
# 3. Build options (user toggles)
# ============================================================================
option(ENABLE_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)
option(ENABLE_ASAN "Enable AddressSanitizer (+LeakSan if available)" ON)
option(ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" ON)
option(ENABLE_TSAN "Enable ThreadSanitizer (mutually exclusive with ASAN)" OFF)
option(ENABLE_LTO "Enable Link-Time Optimization (IPO) in Release/RelWithDebInfo" OFF)
option(ENABLE_CLANG_TIDY "Enable clang-tidy if available" OFF)
option(ENABLE_CONSTEXPR_LIMIT_TWEAKS "Increase compiler constexpr evaluation limits (may slow builds)" ON)

# ============================================================================
# 4. Helper logic
# ============================================================================
# 4.1 Warnings
set(PROJECT_WARNING_FLAGS)
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    list(APPEND PROJECT_WARNING_FLAGS -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion)
elseif (MSVC)
    list(APPEND PROJECT_WARNING_FLAGS /W4 /permissive-)
endif ()
if (ENABLE_WARNINGS_AS_ERRORS)
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        list(APPEND PROJECT_WARNING_FLAGS -Werror)
    elseif (MSVC)
        list(APPEND PROJECT_WARNING_FLAGS /WX)
    endif ()
endif ()

# 4.2 Sanitizers (applied to all build targets we create)
if (ENABLE_ASAN AND ENABLE_TSAN)
    message(FATAL_ERROR "ENABLE_ASAN and ENABLE_TSAN are mutually exclusive.")
endif ()
set(SANITIZER_COMPILE_FLAGS)
set(SANITIZER_LINK_FLAGS)
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif ()
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    if (ENABLE_ASAN AND (CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebInfo"))
        list(APPEND SANITIZER_COMPILE_FLAGS -fsanitize=address -fno-omit-frame-pointer)
        list(APPEND SANITIZER_LINK_FLAGS -fsanitize=address)
    elseif (ENABLE_TSAN AND (CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebInfo"))
        list(APPEND SANITIZER_COMPILE_FLAGS -fsanitize=thread -fno-omit-frame-pointer)
        list(APPEND SANITIZER_LINK_FLAGS -fsanitize=thread)
    endif ()
    if (ENABLE_UBSAN AND (CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebInfo"))
        list(APPEND SANITIZER_COMPILE_FLAGS -fsanitize=undefined)
        list(APPEND SANITIZER_LINK_FLAGS -fsanitize=undefined)
    endif ()
endif ()

# 4.3 Constexpr evaluation limit tweaks
set(CONSTEXPR_LIMIT_FLAGS)
if (ENABLE_CONSTEXPR_LIMIT_TWEAKS)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")  # Matches Clang and AppleClang
        list(APPEND CONSTEXPR_LIMIT_FLAGS -fconstexpr-steps=200000000 -fconstexpr-depth=4096)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        list(APPEND CONSTEXPR_LIMIT_FLAGS -fconstexpr-ops-limit=1000000000 -fconstexpr-depth=4096 -fconstexpr-loop-limit=100000000)
    elseif (MSVC)
        list(APPEND CONSTEXPR_LIMIT_FLAGS /constexpr:steps200000000 /constexpr:depth4096)
    endif ()
endif ()

# Global fallback (affects every target, including OBJECT libs)
add_compile_options(${PROJECT_WARNING_FLAGS} ${SANITIZER_COMPILE_FLAGS} ${CONSTEXPR_LIMIT_FLAGS})
add_link_options(${SANITIZER_LINK_FLAGS})


# 4.5 LTO (IPO)
if (ENABLE_LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT ipo_ok OUTPUT ipo_msg)
    if (ipo_ok)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON)
    else ()
        message(WARNING "IPO/LTO requested but not supported: ${ipo_msg}")
    endif ()
endif ()

# ============================================================================
# 5. Dependencies
# ============================================================================
find_package(Boost 1.70 REQUIRED)
include(FetchContent)
FetchContent_Declare(
        googletest
        # Specify the commit you depend on and update it regularly.
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
FetchContent_MakeAvailable(googletest)

# ============================================================================
# 6. Targets
# ============================================================================
# 6.1 Common library
add_library(euler_common STATIC
        src/common/util.cpp
)
target_include_directories(euler_common PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(euler_common PUBLIC Boost::boost)

# 6.2 Problems aggregation
add_subdirectory(src/problems)
get_property(PROBLEM_OBJECTS GLOBAL PROPERTY PROBLEM_OBJECTS)

# 6.3 Main executable
add_executable(euler src/main.cpp ${PROBLEM_OBJECTS})
target_link_libraries(euler PRIVATE euler_common)
target_compile_definitions(euler PRIVATE ANSWERS_TXT_PATH="${CMAKE_SOURCE_DIR}/answers.txt")

# 6.4 Tests
include(CTest)
add_subdirectory(tests)


# ============================================================================
# 7. Tooling
# ============================================================================
if (ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_EXE NAMES clang-tidy)
    if (CLANG_TIDY_EXE)
        message(STATUS "clang-tidy enabled: ${CLANG_TIDY_EXE}")
        set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    else ()
        message(WARNING "ENABLE_CLANG_TIDY=ON but clang-tidy not found")
    endif ()
endif ()

# ============================================================================
# 8. Developer summary
# ============================================================================
message(STATUS "Config summary:")
message(STATUS "  Build type              : ${CMAKE_BUILD_TYPE}")
message(STATUS "  Warnings as errors      : ${ENABLE_WARNINGS_AS_ERRORS}")
message(STATUS "  ASan                    : ${ENABLE_ASAN}")
message(STATUS "  UBSan                   : ${ENABLE_UBSAN}")
message(STATUS "  TSan                    : ${ENABLE_TSAN}")
message(STATUS "  LTO/IPO                 : ${ENABLE_LTO}")
message(STATUS "  clang-tidy              : ${ENABLE_CLANG_TIDY}")
message(STATUS "  Constexpr limit tweaks  : ${ENABLE_CONSTEXPR_LIMIT_TWEAKS}")
message(STATUS "  Problem objects linked  : ${PROBLEM_OBJECTS}")
