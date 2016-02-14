project(ProjectAscension C CXX)

# ---------- Setup output Directories -------------------------
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
# --------- Setup the Executable output Directory -------------
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
# --------- Setup the Executable output Directory -------------
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

if(WIN32)
    cmake_minimum_required(VERSION 3.1.0)
    set(CMAKE_CXX_STANDARD 11)
else()
    cmake_minimum_required(VERSION 2.8.12)
    add_compile_options(-std=c++11 -Wall)
endif()

if(NOT NO_TESTS)
    enable_testing(true)
    add_subdirectory(tests)
endif()

if(NOT ONLY_TESTS)
    add_subdirectory(src)
endif()
