if(WIN32)
    cmake_minimum_required(VERSION 3.1.0)
    set(CMAKE_CXX_STANDARD 11)
else()
    cmake_minimum_required(VERSION 2.8.12)
    add_compile_options(-std=c++11)
endif()


# --- BUILD CONFIGURATION ---

project(ProjectAscension C CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

include(${CMAKE_CURRENT_SOURCE_DIR}/ProjectAscension.cmake)

set(GUI_TYPE "")

if(WIN32)
    #Additional configuration for Windows systems.
    # TODO: Get working "Release" config for multi-configuration MSVC builds.

    # set(GUI_TYPE WIN32)
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

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

qt5_wrap_ui(FORMS_HEADERS ${FORMS})
qt5_add_resources(UI_RESOURCES ${RESOURCES})
include_directories(
    # ${CMAKE_CURRENT_BINARY_DIR}
    ${Qt5Gui_INCLUDE_DIRS}
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Network_INCLUDE_DIRS}
    ${Qt5Sql_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)
add_executable(ProjectAscension
    ${GUI_TYPE}
    ${SOURCE}
	${FORMS_HEADERS}
	${UI_RESOURCES}
)
target_link_libraries(ProjectAscension
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
	Qt5::Network
    Qt5::Sql
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

if(WIN32)
    target_link_libraries(ProjectAscension
        user32.lib
        gdi32.lib
        dwmapi.lib
    )

    if(MSVC)
        set(WIN_LINKFLAGS \"${CMAKE_CURRENT_SOURCE_DIR}/res/launcher.res\")
        set_target_properties(ProjectAscension PROPERTIES LINK_FLAGS ${WIN_LINKFLAGS})
        set_target_properties(ProjectAscension PROPERTIES COMPILE_FLAGS "/EHsc")
    endif(MSVC)
endif()

if(WITH_TESTS)
    enable_testing()
    add_executable(test_runner
        src/database.cpp
        tests/test.cpp)
    target_link_libraries(test_runner
        Qt5::Sql)
    add_test(test_runner test_runner)
    foreach(FILE
            appinfo.vdf
            game)
    add_custom_command(TARGET ProjectAscension POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/tests/${FILE} $<TARGET_FILE_DIR:ProjectAscension>)
        endforeach()
endif(WITH_TESTS)

# --- RUNTIME DEPENDENCIES ---
# Only necessary on Windows.

if(WIN32)
    cmake_policy(SET CMP0026 OLD)

    get_target_property(BuildLocation ProjectAscension LOCATION)
    get_filename_component(TargetDirectory ${BuildLocation} PATH)

    if (EXISTS ${Qt5Core_DIR}/../../../bin/icuin53.dll)
        set(ICU_VER 53)
    else()
        set(ICU_VER 54)
    endif()
    
    string(COMPARE EQUAL "${CMAKE_BUILD_TYPE}" Release _cmp)
    if (_cmp)
    	set(DEP_CHAR "")
    else()
    	set(DEP_CHAR "d")
    endif()

    foreach(DEPENDENCY
        Qt5Widgetsd.dll
        Qt5Guid.dll
        Qt5Cored.dll
        Qt5Networkd.dll
        Qt5Sqld.dll
        Qt5Svgd.dll
        Qt5OpenGLd.dll
        Qt5Widgets.dll
        Qt5Gui.dll
        Qt5Core.dll
        Qt5Network.dll
        Qt5Sql.dll
        Qt5Svg.dll
        Qt5OpenGL.dll
        icuin${ICU_VER}.dll
        icuuc${ICU_VER}.dll
        icudt${ICU_VER}.dll
    )
        add_custom_command(TARGET ProjectAscension POST_BUILD
            # Copy the given dependency into the executable directory.
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Qt5Core_DIR}/../../../bin/${DEPENDENCY} ${TargetDirectory}
            COMMENT "Copying ${DEPENDENCY} into '${TargetDirectory}/'" VERBATIM
        )
    endforeach()
endif()
