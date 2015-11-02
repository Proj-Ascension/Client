#include "uplay_drm.h"
#include <QStandardPaths>
#include <QSettings>

UplayDRM::UplayDRM() : DRMType("<b>Uplay</b>"){}

void UplayDRM::checkUplayExists()
{
    QDir uplayFolder;
    QDir uplayRoot = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation).append("/Ubisoft Game Launcher");
    bool loggedIn = false;
    if (uplayRoot.exists())
    {
        std::ifstream uplaySettings(uplayRoot.filePath("settings.yml").toLocal8Bit().constData(), std::ifstream::in);
        std::string line;
        if (uplaySettings.is_open())
        {
            while (std::getline(uplaySettings, line))
            {
                if (line.find("user") != std::string::npos)
                {
                    loggedIn = true;
                }
                if (line.find("game_installation_path") != std::string::npos)
                {
                    std::vector<std::string> strSplit;
                    boost::split(strSplit, line, boost::is_any_of("\""));
                    uplayFolder = QDir(QString::fromStdString(strSplit.at(1)));
                    break;
                }
            }
        }

        if (loggedIn)
        {
            if (uplayFolder == QDir("."))
            {
                QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Uplay", QSettings::NativeFormat);
                if (!settings.value("InstallLocation").isNull())
                {
                    uplayFolder = QDir(settings.value("InstallLocation").toString());
                }
            }
        }

    }

    if (uplayFolder.filePath("").trimmed() != "" && uplayFolder != QDir(".") && uplayFolder.exists())
    {
        statusLabel->setPixmap(QPixmap(":/SystemMenu/Icons/Tick.svg"));
        descLabel = new QLabel("Uplay found in " + uplayFolder.filePath(""));
        this->setIsInstalled();
        this->setRootDir(uplayFolder);
    }
    else
    {
        statusLabel->setPixmap(QPixmap(":/SystemMenu/Icons/Cross.svg"));
        descLabel = new QLabel("Uplay not found on the system. Verify installation and try again.");
    }
}
