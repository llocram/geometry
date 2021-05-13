cmake_minimum_required(VERSION 3.3.0...3.16.1)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(geometry LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check compiler requirements (basically, a C++17 compliant compiler)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "7.5")
    message(FATAL_ERROR "Insufficient gcc version: 7.5 required")
  endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "CLANG")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "7.0.0")
    message(FATAL_ERROR "Insufficient clang version: 7.0.0 required")
  endif()
endif()

# Analyze build options
option(WITH_ASAN "Enable address sanitizer (Linux / macOS only)" OFF)
option(WITH_TESTS "Enable building of unit tests" ON)

if(WITH_TESTS)
    message(STATUS "Building tests: Yes")
    enable_testing()
else()
    message(STATUS "Building tests: No")
endif()

# Set Version
set(GEOMETRY_VERSION_MAJOR "0")
set(GEOMETRY_VERSION_MINOR "0")
set(GEOMETRY_VERSION_PATCH "1")
set(GEOMETRY_VERSION "${GEOMETRY_VERSION_MAJOR}.${GEOMETRY_VERSION_MINOR}.${GEOMETRY_VERSION_PATCH}")

# Build type
set(DEFAULT_BUILD_TYPE "Debug")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}")
endif() 
message(STATUS "Setting up build for geometry v${GEOMETRY_VERSION} (${CMAKE_BUILD_TYPE})")

# Retrieve git HEAD revision hash
set(GIT_HEAD_OVERRIDE "" CACHE STRING "Manually set the Git HEAD hash when missing (eg, when no .git folder exists)")
execute_process(COMMAND git rev-parse --short=7 HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_HEAD
        ERROR_QUIET)
string(STRIP "${GIT_HEAD}" GIT_HEAD)
if(GIT_HEAD STREQUAL "" AND NOT GIT_HEAD_OVERRIDE STREQUAL "")
    string(SUBSTRING "${GIT_HEAD_OVERRIDE}" 0 7 GIT_HEAD)
elseif(EXISTS ${CMAKE_SOURCE_DIR}/.gitrev)
    file(READ ${CMAKE_SOURCE_DIR}/.gitrev GIT_HEAD)
endif()
message(STATUS "Found Git HEAD Revision: ${GIT_HEAD}")

# Compiler warnings
macro(add_gcc_compiler_cxxflags FLAGS)
    if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS}")
    endif()
endmacro(add_gcc_compiler_cxxflags)

add_gcc_compiler_cxxflags(
    "-Wall -Wextra -Wpedantic -Wundef -Wpointer-arith -Wno-long-long "
    "-Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused " 
    "-Woverload-virtual -Wconversion -Wsign-conversion -Wdouble-promotion "
    "-Wformat=2 -Wduplicated-cond -Wduplicated-branches "
    "-Wlogical-op -Wuseless-cast -Wlifetime")

# ASAN
if(WITH_ASAN)
    if(NOT (CMAKE_SYSTEM_NAME STREQUAL "Linux" OR APPLE))
        message(FATAL_ERROR "WITH_ASAN is only supported on Linux / macOS at the moment.")
    endif()

    message(STATUS "ASan enabled")
    add_gcc_compiler_cxxflags("-fsanitize=address -DWITH_ASAN")

    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        if(NOT (CMAKE_COMPILER_IS_GNUCXX AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9))
            add_gcc_compiler_cxxflags("-fsanitize=leak -DWITH_LSAN")
        endif()
    endif()
else()
    message(STATUS "ASan disabled")
endif()

add_subdirectory(examples)

if (WITH_TESTS)
  add_subdirectory(test)
endif()
