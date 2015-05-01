TEMPLATE = app
RC_FILE = Launcher.rc
win32:RC_ICONS += Icons/Ascension_Icon.ico
CONFIG += app_bundle
LIBS += gdi32.lib dwmapi.lib

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

include("QtWinMigrate.pri")
include("Common.pri")

RESOURCES += \
    Icons.qrc

OTHER_FILES += \
    BorderlessWindow.css

FORMS += \
    Library.ui \
    Browser.ui
