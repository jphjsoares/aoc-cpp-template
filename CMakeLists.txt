cmake_minimum_required(VERSION 3.14)
project(AdventOfCode2025 VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable testing
enable_testing()

# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Find CURL for input fetcher
find_package(CURL REQUIRED)

# Common utilities library
add_library(common
    src/common/utils.cpp
    src/common/input_fetcher.cpp
)
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(common PUBLIC CURL::libcurl)

# Dynamically discover and add year/day subdirectories
file(GLOB YEAR_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src/[0-9][0-9][0-9][0-9]")
set(ALL_DAY_LIBS "")

foreach(YEAR_DIR ${YEAR_DIRS})
    get_filename_component(YEAR ${YEAR_DIR} NAME)
    file(GLOB DAY_DIRS "${YEAR_DIR}/day_[0-9]*")

    foreach(DAY_DIR ${DAY_DIRS})
        if(EXISTS "${DAY_DIR}/CMakeLists.txt")
            get_filename_component(DAY ${DAY_DIR} NAME)
            message(STATUS "Adding ${YEAR}/${DAY}")
            add_subdirectory(src/${YEAR}/${DAY})
            list(APPEND ALL_DAY_LIBS ${YEAR}_${DAY})
        endif()
    endforeach()
endforeach()

# Main runner executable
add_executable(aoc main.cpp)
target_link_libraries(aoc
    common
    ${ALL_DAY_LIBS}
)
target_include_directories(aoc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Copy .env file to build directory if it exists
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.env")
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/.env"
        "${CMAKE_CURRENT_BINARY_DIR}/.env"
        COPYONLY
    )
endif()

# clang-format target for Google C++ style enforcement
find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    message(STATUS "clang-format found: ${CLANG_FORMAT}")

    # Collect all C++ source files
    file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )

    # Format target - applies formatting to all files
    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i -style=file ${ALL_CXX_SOURCE_FILES}
        COMMENT "Running clang-format to format all C++ files"
    )

    # Check-format target - verifies formatting without modifying files
    add_custom_target(check-format
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts/check_format.sh
        COMMENT "Checking C++ code formatting"
    )
else()
    message(WARNING "clang-format not found. 'make format' target will not be available.")
endif()
