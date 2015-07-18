#pragma once

#include <QApplication>
#include <QFontDatabase>
#include <QSettings>
#include <QMessageBox>
#include <QFile>

namespace entryPoint
{
void initSettings(QApplication* application)
{
    QString configFile = "config.ini", paletteFile = "palette.ini";
    QSettings config(configFile, QSettings::IniFormat), palette(paletteFile, QSettings::IniFormat);

    config.setPath(QSettings::IniFormat, QSettings::UserScope, application->applicationDirPath());
    palette.setPath(QSettings::IniFormat, QSettings::UserScope, application->applicationDirPath());

    if (!QFile(configFile).exists() && config.isWritable())
    {
        // TODO: Set default config settings.
    }
    if (!QFile(paletteFile).exists() && palette.isWritable())
    {
        palette.beginGroup("Primary");

        palette.setValue("ActiveElement", "#FFFFFF");
        palette.setValue("ActiveSelection", "#242424");
        palette.setValue("HoverSelection", "#1B1B1B");
        palette.setValue("LightText", "#FFFFFF");
        palette.setValue("DarkText", "#242424");
        palette.setValue("SubText", "#8B8B8B");
        palette.setValue("PrimaryBase", "#282828");
        palette.setValue("SecondaryBase", "#1F1F1F");
        palette.setValue("TertiaryBase", "#181818");
        palette.setValue("DarkestBase", "#0F0F0F");
        palette.endGroup();

        palette.beginGroup("Accent");

        palette.setValue("LightAccent", "#68AD28");
        palette.setValue("MediumAccent", "#589828");
        palette.setValue("DarkAccent", "#487D28");
        palette.endGroup();
    }
}

void initFonts(QApplication* application)
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
}
