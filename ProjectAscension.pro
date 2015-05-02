TEMPLATE = app
RC_FILE = Launcher.rc
CONFIG += app_bundle

win32 {
    win32:RC_ICONS += Icons/Ascension_Icon.ico
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
    Source/MainWindow.h \
    Source/QMainPanel.h \
    Source/Game.h \
    Source/TabLabel.h \
    Source/Library.h \
    Source/Browser.h

SOURCES += \
    Source/Main.cpp \
    Source/MainWindow.cpp \
    Source/QMainPanel.cpp \
    Source/TabLabel.cpp \
    Source/Library.cpp \
    Source/Browser.cpp

INCLUDEPATH += \
    Source\

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
