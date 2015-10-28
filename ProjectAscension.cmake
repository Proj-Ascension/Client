set(SOURCE_CORE
    src/main.cpp
    src/init.h
    src/database.cpp
    src/database.h
)

set(LIBS
    src/libs/catch.hpp
    src/libs/steam_vdf_parse.hpp
)

set(GUI_CORE
    src/gui/main_panel.cpp
    src/gui/main_panel.h
    src/gui/sidebar.cpp
    src/gui/sidebar.h
    src/gui/tab_widget.cpp
    src/gui/tab_widget.h
)

set(GUI_STACK
    src/gui/stack/library.cpp
    src/gui/stack/library.h
    src/gui/stack/news.cpp
    src/gui/stack/news.h
    src/gui/stack/news_feed_widget.cpp
    src/gui/stack/news_feed_widget.h
    src/gui/stack/settings.cpp
    src/gui/stack/settings.h
)

set(GUI_WIZARDS
    src/gui/wizards/drm_type.cpp
    src/gui/wizards/drm_type.h
    src/gui/wizards/add_game_wizard.cpp
    src/gui/wizards/add_game_wizard.h
    src/gui/wizards/drm_setup_wizard.cpp
    src/gui/wizards/drm_setup_wizard.h
)

set(FORMS
    res/gui/library.ui
)

set(RESOURCES
    res/icons.qrc
    res/fonts.qrc
    res/styles.qrc
    res/elements.qrc
)

if(WIN32)
    set(GUI_CORE ${GUI_CORE}
        src/gui/win_window.cpp
        src/gui/win_window.h
        src/gui/win_panel.cpp
        src/gui/win_panel.h
    )

    set(GUI_BORDERLESS
        src/gui/borderless/borderless_window.cpp
        src/gui/borderless/borderless_window.h
        src/gui/borderless/qwinhost.cpp
        src/gui/borderless/qwinhost.h
        src/gui/borderless/qwinwidget.cpp
        src/gui/borderless/qwinwidget.h
    )

    if(MSVC)
        source_group("src\\" FILES ${SOURCE_CORE})
        source_group("src\\libs\\" FILES ${LIBS})
        source_group("src\\gui\\" FILES ${GUI_CORE})
        source_group("src\\gui\\borderless\\" FILES ${GUI_BORDERLESS})
        source_group("src\\gui\\stack\\" FILES ${GUI_STACK})
        source_group("src\\gui\\wizards\\" FILES ${GUI_WIZARDS})
    endif()
elseif(UNIX)
    set(GUI_CORE ${GUI_CORE}
        src/gui/unix_window.cpp
        src/gui/unix_window.h
        src/gui/unix_panel.cpp
        src/gui/unix_panel.h
    )

    set(GUI_BORDERLESS "")
else()
    # Suspend the configuration process if the target OS isn't recognized.
    MESSAGE(FATAL_ERROR "Target OS not recognized or supported, aborting CMake process.")
endif()

set(SOURCE
    ${SOURCE_CORE}
    ${LIBS}
    ${GUI_CORE}
    ${GUI_STACK}
    ${GUI_WIZARDS}
    ${GUI_BORDERLESS}
    ProjectAscension.cmake
)

include_directories(
    src
    src/libs
    src/gui
    src/gui/borderless
    src/gui/stack
    src/gui/wizards
)