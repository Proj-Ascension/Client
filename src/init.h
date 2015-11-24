#include <QApplication>
#include <QFontDatabase>
#include <QSettings>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>

void initSettings(QApplication &application)
{

   /* QSettings config(QSettings::IniFormat, QSettings::UserScope, "Project Ascension", "config");
    QSettings rss(QSettings::IniFormat, QSettings::UserScope, "Project Ascension", "rss");
    QSettings palette(QSettings::IniFormat, QSettings::UserScope, "Project Ascension", "palette");*/

    QSettings config(QString("config.ini"), QSettings::IniFormat);
    QSettings rss(QString("rss.ini"), QSettings::IniFormat);
    QSettings palette(QString("palette.ini"), QSettings::IniFormat);

    if (!QFile("config.ini").exists() && config.isWritable())
    {
        // TODO: Set default config settings.
    }

    if (!QFile("rss.ini").exists() && rss.isWritable())
    {
        // TODO: Set default rss
    }

    if (!QFile(palette.fileName()).exists() && palette.isWritable())
    {
        palette.beginGroup("Primary");
        palette.setValue("ActiveElement", "#FFFFFF");
        palette.setValue("InactiveSelection", "#686868");
        palette.setValue("HoverSelection", "#ADADAD");
        palette.setValue("DarkElement", "#4D5250");
        palette.setValue("LightText", "#FFFFFF");
        palette.setValue("DarkText", "#242424");
        palette.setValue("SubText", "#B2B4B3");
        palette.setValue("PrimaryBase", "#282828");
        palette.setValue("SecondaryBase", "#1F1F1F");
        palette.setValue("TertiaryBase", "#131313");
        palette.setValue("DarkestBase", "#0F0F0F");
        palette.endGroup();

        palette.beginGroup("Accent");
        palette.setValue("LightAccent", "#E58F12");
        palette.setValue("MediumAccent", "#895f06");
        palette.setValue("DarkAccent", "#6a4a05");
        palette.endGroup();
    }
}

void initFonts(QApplication &application)
{
    // Font
    QFont mainFont = application.font();
    mainFont.setStyleStrategy(QFont::PreferAntialias);
    application.setFont(mainFont);

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
        QFile res(":/typeface/" + font);
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

QString getConfigurableStyle() {
    QSettings p(QString("palette.ini"), QSettings::IniFormat);
    return "QPushButton {"
           "color: " + p.value("Primary/LightText").toString() + ";"
           "background-color: " + p.value("Primary/DarkElement").toString() + ";"
           "font: SourceSansPro 9;"
           "margin: 0; padding: 0;"
           "border-style: none;}"
           "QPushButton:hover {background-color: " + p.value("Primary/InactiveSelection").toString() + ";}"
           "QPushButton[default='true'] {background-color: " + p.value("Accent/MediumAccent").toString() + ";}"
           "QPushButton[default='true']:hover {background-color: " + p.value("Accent/LightAccent").toString() + ";}";
}
