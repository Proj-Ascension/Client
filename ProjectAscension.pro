TEMPLATE = app
RC_FILE = Launcher.rc
CONFIG += app_bundle
win32 {
    RC_ICONS += Icons/Ascension_Icon.ico
    LIBS += gdi32.lib dwmapi.lib
}

QT += \
    core \
    gui \
    widgets \
    network \
    webkit \
    webkitwidgets

HEADERS += \
    Source/Game.h \
    Source/TabLabel.h \
    Source/Library.h \
    Source/Browser.h


SOURCES += \
    Source/Main.cpp \
    Source/TabLabel.cpp \
    Source/Library.cpp \
    Source/Browser.cpp

win32 {
    HEADERS += \
        Source/WinPanel.h \
        Source/BorderlessWindow.h
    SOURCES += \
        Source/WinPanel.cpp \
        Source/BorderlessWindow.cpp
}

unix {
    HEADERS += \
        Source/UnixPanel.h
    SOURCES += \
        Source/UnixPanel.cpp
}

INCLUDEPATH += \
    Source/

VPATH += \
    BorderlessWindow/

win32 {
    include("BorderlessWindow/QtWinMigrate.pri")
    include("BorderlessWindow/Common.pri")
}

RESOURCES += \
    Icons.qrc

OTHER_FILES += \
    BorderlessWindow.css

FORMS += \
    Library.ui \
    Browser.ui
