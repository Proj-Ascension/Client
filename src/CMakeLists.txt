project(Client C CXX)

set(SOURCE_CORE
    main.cpp
    init.h
    database.cpp
    database.h
    game.h
    widgets.h
)

set(LIBS
    libs/steam_vdf_parse.hpp
    libs/util.hpp
)

set(DRM
    drm/drm_all.h
    drm/drm_type.cpp
    drm/drm_type.h
    drm/steam_drm.cpp
    drm/steam_drm.h
    drm/origin_drm.cpp
    drm/origin_drm.h
    drm/uplay_drm.cpp
    drm/uplay_drm.h
)

set(GUI_CORE
    gui/main_panel.cpp
    gui/main_panel.h
    gui/sidebar.cpp
    gui/sidebar.h
    gui/tab_widget.cpp
    gui/tab_widget.h
)

set(GUI_DIALOGS
    gui/dialogs/ascension_dialog.cpp
    gui/dialogs/ascension_dialog.h
)

set(GUI_STACK
    gui/stack/library.cpp
    gui/stack/library.h
    gui/stack/news.cpp
    gui/stack/news.h
    gui/stack/news_feed_widget.cpp
    gui/stack/news_feed_widget.h
    gui/stack/settings.cpp
    gui/stack/settings.h
)

set(GUI_WIZARDS
    gui/wizards/add_game_wizard.cpp
    gui/wizards/add_game_wizard.h
    gui/wizards/drm_setup_wizard.cpp
    gui/wizards/drm_setup_wizard.h
)

set(RESOURCES
    ../res/icons.qrc
    ../res/fonts.qrc
    ../res/styles.qrc
    ../res/elements.qrc
)


set(GUI_TYPE "")

if(WIN32)
#    set(GUI_TYPE WIN32)

    set(GUI_CORE ${GUI_CORE}
        gui/win_window.cpp
        gui/win_window.h
        gui/win_panel.cpp
        gui/win_panel.h
    )

    set(GUI_BORDERLESS
        gui/borderless/borderless_window.cpp
        gui/borderless/borderless_window.h
        gui/borderless/qwinhost.cpp
        gui/borderless/qwinhost.h
        gui/borderless/qwinwidget.cpp
        gui/borderless/qwinwidget.h
    )

    if(MSVC)
        source_group("src\\" FILES ${SOURCE_CORE})
        source_group("src\\libs\\" FILES ${LIBS})
        source_group("src\\gui\\" FILES ${GUI_CORE})
        source_group("src\\gui\\borderless\\" FILES ${GUI_BORDERLESS})
        source_group("src\\gui\\stack\\" FILES ${GUI_STACK})
        source_group("src\\gui\\wizards\\" FILES ${GUI_WIZARDS})
		source_group("src\\drm\\" FILES ${DRM})
    endif()
elseif(UNIX)
    set(GUI_CORE ${GUI_CORE}
        gui/unix_window.cpp
        gui/unix_window.h
        gui/unix_panel.cpp
        gui/unix_panel.h
    )

    set(GUI_BORDERLESS "")

    # Additional configuration for Unix systems.
    # TODO: Get working icon / bundle for OSX systems without bricking the build.

    # if(APPLE)
    #     set(GUI_TYPE MACOSX_BUNDLE)
    # endif(APPLE)
else()
    # Suspend the configuration process if the target OS isn't recognized.
    MESSAGE(FATAL_ERROR "Target OS not recognized or supported, aborting CMake process.")
endif()

set(GUI
    ${GUI_CORE}
    ${GUI_DIALOGS}
    ${GUI_STACK}
    ${GUI_WIZARDS}
    ${GUI_BORDERLESS})

set(SOURCE
    ${SOURCE_CORE}
    ${LIBS}
    ${DRM}
    ${GUI}
)

include_directories(
    .
    drm
    libs
    gui
    gui/dialogs
    gui/borderless
    gui/stack
    gui/wizards
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 5.3 COMPONENTS Core Gui Network Sql Widgets)
find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

qt5_add_resources(UI_RESOURCES ${RESOURCES})
include_directories(
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
    ${QT_MAIN}
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    )
qt5_use_modules(${PROJECT_NAME}
    Widgets
    Core
    Gui
    Network
    Sql)

if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        user32.lib
        gdi32.lib
        dwmapi.lib
        )

    if(MSVC)
        set(WIN_LINKFLAGS \"${CMAKE_CURRENT_SOURCE_DIR}/../res/launcher.res\")
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
            COMMAND CMD /C "${WINDEPLOY} $<$<OR:$<CONFIG:Release>,$<CONFIG:MinSizeRel>,$<CONFIG:RelWithDebInfo>>:--release> --no-translations  $<TARGET_FILE:${PROJECT_NAME}> --dir $<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins && move $<TARGET_FILE_DIR:${PROJECT_NAME}>\\plugins\\*.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>"
            )
    else()
        message(WARNING "Unable to find Qt Windows deployment tool at ${Qt5Core_DIR}/../../../bin, no runtime dependencies will be copied.")
    endif()
endif()
