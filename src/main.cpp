#include "init.h"
#include "gui/wizards/drm_setup_wizard.h"

#ifdef Q_OS_WIN
#include "gui/borderless/borderless_window.h"

#else
#include "gui/unix_window.h"

#endif

int main(int argc, char* argv[])
{
    QCoreApplication::addLibraryPath("./");

    QApplication::setStyle("fusion");
    QApplication* application = new QApplication(argc, argv);
    bool dbExists = QFile("ascension.db").exists();
    DRMSetupWizard* wiz = new DRMSetupWizard();

    #ifndef Q_OS_WIN
        // dynamic loading of the icon under Linux/UNIX
        application->setWindowIcon(QIcon(":/system_menu/icons/ascension_icon.ico"));
    #endif

    // Global stylesheet
    QFile stylesheet(":/styles/pa_client.css");
    if (stylesheet.open(QFile::ReadOnly))
    {
        QString styleSheet = stylesheet.readAll();
        application->setStyleSheet(styleSheet + getConfigurableStyle());
    }

    initSettings(*application);
    initFonts(*application);

    #ifdef Q_OS_WIN
        // Background color
        // This is only for WinApi window, Qt widgets use BorderlessWindow.css stylesheet
        HBRUSH windowBackground = CreateSolidBrush(RGB(15, 15, 15));

        // Create a Win window
        BorderlessWindow window(application, windowBackground, 1152, 648);
        window.setMinimumSize(830, 550);
    #else
        // Create a Unix window
        UnixWindow window;
        window.setMinimumSize(830, 550);
    #endif

    // Launch
    if (!dbExists)
    {
        wiz->show();
    }

    application->exec();
    return 0;
}
