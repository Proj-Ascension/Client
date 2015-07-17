#include "Init.h"

#include <QApplication>
#include <QFile>

#ifdef Q_OS_WIN
#include "BorderlessWindow.h"
#else
#include "UnixWindow.h"
#endif

int main(int argc, char* argv[])
{
    QApplication::setStyle("fusion");
    QApplication* application = new QApplication(argc, argv);

    #ifndef Q_OS_WIN
        // dynamic loading of the icon under Linux/UNIX
        application->setWindowIcon(QIcon(":/SystemMenu/Icons/Ascension_Icon.ico"));
    #endif

    // Stylesheet
    QFile stylesheet(":/Styles/PAClient.css");
    if (stylesheet.open(QFile::ReadOnly))
    {
        QString styleSheet = stylesheet.readAll();
        application->setStyleSheet(styleSheet);
    }

    entryPoint::initSettings();
    entryPoint::initFonts(application);

    #ifdef Q_OS_WIN
        // Background color
        // This is only for WinApi window, Qt widgets use BorderlessWindow.css stylesheet
        HBRUSH windowBackground = CreateSolidBrush(RGB(34, 38, 47));

        // Create a Win window
        BorderlessWindow window(application, windowBackground, 1152, 648);
        window.setMinimumSize(830, 550);
    #else
        // Create a Unix window
        UnixWindow window;
        window.setMinimumSize(830, 550);
    #endif

    // Launch
    application->exec();

    return 0;
}
