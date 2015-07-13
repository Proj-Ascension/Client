#include <QMessageBox>
#include <QFontDatabase>
#include <QFile>
#include <QSize>
#include <QApplication>

#ifdef Q_OS_WIN
#include "BorderlessWindow.h"
#else
#include "UnixWindow.h"
#endif

void g_loadFonts(QApplication* application)
{
    // Font
    QFont mainFont = application->font();
    mainFont.setStyleStrategy(QFont::PreferAntialias);
    application->setFont(mainFont);

    // Dynamically load fonts
    QStringList list;
    list << "Montserrat-Black.ttf" <<
        "Montserrat-Bold.ttf" <<
        "Montserrat-Light.ttf" <<
        "Montserrat-Regular.ttf" <<
        "Montserrat-Thin.ttf" <<
        "Roboto-BlackItalic.ttf" <<
        "Roboto-Black.ttf" <<
        "Roboto-BoldItalic.ttf" <<
        "Roboto-Bold.ttf" <<
        "Roboto-Italic.ttf" <<
        "Roboto-LightItalic.ttf" <<
        "Roboto-Light.ttf" <<
        "Roboto-MediumItalic.ttf" <<
        "Roboto-Medium.ttf" <<
        "Roboto-Regular.ttf" <<
        "Roboto-ThinItalic.ttf" <<
        "Roboto-Thin.ttf" <<
        "SourceCodePro-Black.ttf" <<
        "SourceCodePro-Bold.ttf" <<
        "SourceCodePro-ExtraLight.ttf" <<
        "SourceCodePro-Light.ttf" <<
        "SourceCodePro-Medium.ttf" <<
        "SourceCodePro-Regular.ttf" <<
        "SourceCodePro-Semibold.ttf" <<
        "SourceSansPro-BlackItalic.ttf" <<
        "SourceSansPro-Black.ttf" <<
        "SourceSansPro-BoldItalic.ttf" <<
        "SourceSansPro-Bold.ttf" <<
        "SourceSansPro-ExtraLightItalic.ttf" <<
        "SourceSansPro-ExtraLight.ttf" <<
        "SourceSansPro-Italic.ttf" <<
        "SourceSansPro-LightItalic.ttf" <<
        "SourceSansPro-Light.ttf" <<
        "SourceSansPro-Regular.ttf" <<
        "SourceSansPro-SemiboldItalic.ttf" <<
        "SourceSansPro-Semibold.ttf";

    int fontID(-1);
    bool fontWarningShown(false);
    for (auto font : list)
    {
        QFile res(":/Typeface/" + font);
        if (!res.open(QIODevice::ReadOnly))
        {
            if (!fontWarningShown)
            {
                QMessageBox::warning(0, "Application", (QString) "Warning: Unable to load font " + QChar(0x00AB) + font + QChar(0x00BB) + ".");
                fontWarningShown = true;
            }
        }
        else
        {
            fontID = QFontDatabase::addApplicationFontFromData(res.readAll());
            if (fontID == -1 && !fontWarningShown)
            {
                QMessageBox::warning(0, "Application", (QString) "Warning: Unable to load font " + QChar(0x00AB) + font + QChar(0x00BB) + ".");
                fontWarningShown = true;
            }
        }
    }

}

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

    g_loadFonts(application);

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
        window.resize(QSize(1152, 648));
        window.setMinimumSize(830, 550);

    #endif

    // Launch
    application->exec();

    return 0;
}
