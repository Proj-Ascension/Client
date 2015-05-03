TEMPLATE = app
RC_FILE = Launcher.rc
CONFIG += app_bundle

CONFIG += c++11

win32 {
    RC_ICONS += Icons/Ascension_Icon.ico
    LIBS += -lgdi32 -ldwmapi
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

release: DESTDIR = build/release
debug:   DESTDIR = build/debug

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

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

OTHER_FILES +=

FORMS += \
    Library.ui \
    Browser.ui

DISTFILES += \
    BorderlessWindow/BorderlessWindow.css
