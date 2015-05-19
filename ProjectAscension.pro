TEMPLATE = app
RC_FILE = Resource/Launcher.rc
CONFIG += app_bundle

CONFIG += c++11

win32 {
    RC_ICONS += Resource/Icons/Ascension_Icon.ico
    LIBS += -lgdi32 -ldwmapi
}

QT += \
    core \
    gui \
    widgets \
    network \
    webkit \
    webkitwidgets \
    sql

HEADERS += \
    Source/TabLabel.h \
    Source/Library.h \
    Source/Browser.h \
    Source/MainPanel.h \
    Source/Database.h

SOURCES += \
    Source/Main.cpp \
    Source/TabLabel.cpp \
    Source/Library.cpp \
    Source/Browser.cpp \
    Source/MainPanel.cpp \
    Source/Database.cpp

win32 {
    HEADERS += \
        Source/BorderlessWindow.h \
        Source/WinWindow.h \
        Source/WinPanel.h

    SOURCES += \
        Source/BorderlessWindow.cpp \
        Source/WinWindow.cpp \
        Source/WinPanel.cpp
}

unix {
    HEADERS += \
        Source/UnixWindow.h \
        Source/UnixPanel.h

    SOURCES += \
        Source/UnixWindow.cpp \
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
    Resource/Icons.qrc \
    Resource/Fonts.qrc \
    Resource/Styles.qrc

OTHER_FILES +=

FORMS += \
    Resource/UI/Library.ui \
    Resource/UI/Browser.ui

DISTFILES += \
    Resource/PAClient.css \
    Resource/Content.css
