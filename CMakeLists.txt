cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 11)

# --- BUILD CONFIGURATION ---

project(ProjectAscension)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(SOURCE
    Source/Main.cpp
    Source/TabLabel.cpp
    Source/Library.cpp
    Source/Browser.cpp
    Source/MainPanel.cpp
    Source/Database.cpp
)

set(HEADERS
    Source/TabLabel.h
    Source/Library.h
    Source/Browser.h
    Source/MainPanel.h
    Source/Database.h
)

set(FORMS
    Resource/UI/Library.ui
    Resource/UI/Browser.ui
)

set(RESOURCES
    Resource/Icons.qrc
    Resource/Fonts.qrc
    Resource/Styles.qrc
)

set(GUI_TYPE "")

if(WIN32)
    #Additional configuration for Windows systems.
    set(GUI_TYPE WIN32)
    
    set(SOURCE ${SOURCE}
        Source/BorderlessWindow.cpp
        Source/WinWindow.cpp
        Source/WinPanel.cpp
        BorderlessWindow/QWinHost.cpp
        BorderlessWindow/QWinWidget.cpp
    )
    
    set(HEADERS ${HEADERS}
        Source/BorderlessWindow.h
        Source/WinWindow.h
        Source/WinPanel.h
        BorderlessWindow/QWinHost.h
        BorderlessWindow/QWinWidget.h
    )
elseif(UNIX)
    # Additional configuration for Unix systems.
    if(APPLE)
        set(GUI_TYPE MACOSX_BUNDLE)
    endif(APPLE)
    
    set(SOURCE ${SOURCE}
        Source/UnixWindow.cpp
        Source/UnixPanel.cpp
    )
    
    set(HEADERS ${HEADERS}
        Source/UnixWindow.h
        Source/UnixPanel.h
    )
else()
    # Suspend the configuration process if the target OS isn't recognized.
    MESSAGE(FATAL_ERROR "Target OS not recognized or supported, aborting CMake process.")
endif()

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5WebKit REQUIRED)
find_package(Qt5WebKitWidgets REQUIRED)
find_package(Qt5Sql REQUIRED)

qt5_wrap_ui(FORMS_HEADERS ${FORMS})
qt5_add_resources(UI_RESOURCES ${RESOURCES})
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
    BorderlessWindow 
    Source
)
add_executable(ProjectAscension 
    ${GUI_TYPE} 
    ${SOURCE} 
    ${OS_SOURCE} 
    ${FORMS_HEADERS} 
    ${UI_RESOURCES}
)
target_link_libraries(ProjectAscension 
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
    Qt5::Network
    Qt5::WebKit
    Qt5::WebKitWidgets
    Qt5::Sql
)
if(WIN32)
    target_link_libraries(ProjectAscension
        user32.lib
        gdi32.lib
        dwmapi.lib
    )
endif()

# --- RUNTIME DEPENDENCIES ---
# Only necessary on Windows

if(WIN32)
    cmake_policy(SET CMP0026 OLD)

    get_target_property(BuildLocation ProjectAscension LOCATION)
    get_filename_component(TargetDirectory ${BuildLocation} PATH)
    
    add_custom_command(TARGET ProjectAscension POST_BUILD
        # Copies runtime dependencies into the executable directory.
    
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Widgetsd.dll           ${TargetDirectory}
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Guid.dll               ${TargetDirectory}
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Cored.dll              ${TargetDirectory}
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Networkd.dll           ${TargetDirectory}
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5WebKitd.dll            ${TargetDirectory}
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5WebKitWidgetsd.dll     ${TargetDirectory}
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Sqld.dll               ${TargetDirectory}
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Sensorsd.dll           ${TargetDirectory}
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Positioningd.dll       ${TargetDirectory}
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Quickd.dll             ${TargetDirectory}
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Qmld.dll               ${TargetDirectory}
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Multimediad.dll        ${TargetDirectory}
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5MultimediaWidgetsd.dll ${TargetDirectory}
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5WebChanneld.dll        ${TargetDirectory}
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5OpenGLd.dll            ${TargetDirectory}
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5PrintSupportd.dll      ${TargetDirectory}
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/icuin53.dll               ${TargetDirectory}
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/icuuc53.dll               ${TargetDirectory}
        COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/icudt53.dll               ${TargetDirectory}
    )
endif()