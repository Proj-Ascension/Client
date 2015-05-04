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

int main(int argc, char *argv[])
{
    QApplication::setStyle("fusion");
    QApplication *application = new QApplication(argc, argv);

    // Stylesheet
    QFile stylesheet(":/SystemMenu/BorderlessWindow/BorderlessWindow.css");
    if (stylesheet.open(QFile::ReadOnly))
    {
        QString styleSheet = stylesheet.readAll();
        application->setStyleSheet(styleSheet);
    }

    // Font
    QFont mainFont = application->font();
    mainFont.setStyleStrategy(QFont::PreferAntialias);
    application->setFont(mainFont);

    // Dynamically load fonts
    QStringList list;
    list << "Sansation_Light.ttf" << "Sansation_Regular.ttf" << "Sansation_Bold.ttf";
    int fontID(-1);
    bool fontWarningShown(false);
    for (QStringList::const_iterator constIterator = list.constBegin(); constIterator != list.constEnd(); ++constIterator)
    {
        QFile res(":/Typeface/Fonts/" + *constIterator);
        if (res.open(QIODevice::ReadOnly) == false)
        {
            if (fontWarningShown == false)
            {
                QMessageBox::warning(0, "Application", (QString)"Warning: Unable to load font " + QChar(0x00AB) + *constIterator + QChar(0x00BB) + ".");
                fontWarningShown = true;
            }
        }
        else
        {
            fontID = QFontDatabase::addApplicationFontFromData(res.readAll());
            if (fontID == -1 && fontWarningShown == false)
            {
                QMessageBox::warning(0, "Application", (QString)"Warning: Unable to load font " + QChar(0x00AB) + *constIterator + QChar(0x00BB) + ".");
                fontWarningShown = true;
            }
        }
    }

#ifdef Q_OS_WIN
    // Background color
    // This is only for WinApi window, Qt widgets use BorderlessWindow.css stylesheet
    HBRUSH windowBackground = CreateSolidBrush(RGB(34, 38, 47));

    // Create window
    BorderlessWindow window(application, windowBackground, 1152, 648);
    window.setMinimumSize(830, 550);
#else
    // Create a Unix window
    UnixWindow window;
    QSize *size = new QSize(1152, 648);
    window.resize(*size);
    window.setMinimumSize(830, 550);
#endif

    // Launch
    application->exec();
}
