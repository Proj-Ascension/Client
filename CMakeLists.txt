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
set(gtest_disable_pthreads on)

include(ProjectAscension.cmake)
add_subdirectory(tests)

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
add_executable(${PROJECT_NAME}
    ${GUI_TYPE}
    ${SOURCE}
    ${FORMS_HEADERS}
    ${UI_RESOURCES}
)
target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    Qt5::Network
    Qt5::Sql
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        user32.lib
        gdi32.lib
        dwmapi.lib
    )

    if(MSVC)
        set(WIN_LINKFLAGS \"${CMAKE_CURRENT_SOURCE_DIR}/res/launcher.res\")
        set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS ${WIN_LINKFLAGS})
        set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/EHsc")
    endif(MSVC)
endif()


# --- RUNTIME DEPENDENCIES ---
# Only necessary on Windows.

if(WIN32)
    find_program(WINDEPLOY windeployqt PATHS ${Qt5Core_DIR}/../../../bin NO_DEFAULT_PATH)

    if (WINDEPLOY)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            # Call windeployqt to copy necessary dependencies into the executable path.
            COMMAND CMD /C "call ${Qt5Core_DIR}/../../../bin/qtenv2.bat && ${WINDEPLOY} $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>,$<CONFIG:RelWithDebInfo>>:--release> --no-translations  $<TARGET_FILE:${PROJECT_NAME}> --dir $<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins && move $<TARGET_FILE_DIR:${PROJECT_NAME}>\\plugins\\*.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>"
        )
    else()
        message(WARNING "Unable to find Qt Windows deployment tool at ${Qt5Core_DIR}/../../../bin, no runtime dependencies will be copied.")
    endif()
endif()
