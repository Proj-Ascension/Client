if(WIN32)
    cmake_minimum_required(VERSION 3.1.0)
    set(CMAKE_CXX_STANDARD 11)
else()
    cmake_minimum_required(VERSION 2.8.12)
    add_compile_options(-std=c++11)
endif()

# --- BUILD CONFIGURATION ---


if(WITH_TESTS)
    enable_testing(true)
    add_custom_target(check ${CMAKE_TEST_COMMAND} -V)
    add_subdirectory(tests)
endif()

add_subdirectory(src)

set(GUI_TYPE "")

if(WIN32)
    set(GUI_TYPE WIN32)
elseif(UNIX)
    # Additional configuration for Unix systems.
    # TODO: Get working icon / bundle for OSX systems without bricking the build.

    # if(APPLE)
    #     set(GUI_TYPE MACOSX_BUNDLE)
    # endif(APPLE)
else()
    # Suspend the configuration process if the target OS isn't recognized.
    MESSAGE(FATAL_ERROR "Target OS not recognized or supported, aborting CMake process.")
endif()
