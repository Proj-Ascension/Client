#include "drm_type.h"

/** Check if Steam is installed on the current computer, if applicable, and sets some values for later pages to
 * check on.
 * \see checkOriginExists()
 * \see checkUplayExists()
 */
void SteamDRM::checkSteamExists()
{
    QDir steamFolder;

#if defined(__linux__)
    QProcess which;
    which.setProcessChannelMode(QProcess::MergedChannels);
    QStringList args;
    args << "steam";
    which.start("which", args);
    which.waitForFinished();
    if (which.exitCode() == 0)
    {
        steamFolder = QDir(QDir::homePath() + "/.local/share/Steam");
    }
#elif defined(_WIN32) || defined(_WIN64)
    QSettings settings("HKEY_CURRENT_USER\\Software\\Valve\\Steam", QSettings::NativeFormat);
    if (!settings.value("SteamPath").isNull())
    {
        steamFolder = QDir(settings.value("SteamPath").toString()).canonicalPath();
    }
#elif defined(__APPLE__)
    steamFolder = QDir(QDir::home().filePath("Library/Application Support/Steam"));
#endif

    if (steamFolder.filePath("").trimmed() != "" && steamFolder.exists() && steamFolder != QDir("."))
    {
        statusLabel->setPixmap(QPixmap(":SystemMenu/Icons/Tick.svg"));
        descLabel = new QLabel("Steam found in " + steamFolder.filePath(""));
        QDir steamAppsDir = steamFolder.filePath("steamapps");
        if (!steamAppsDir.exists())
        {
            steamAppsDir = steamFolder.filePath("SteamApps");
        }
        pt::ptree libraryFolders;
        pt::read_info(steamAppsDir.filePath("libraryfolders.vdf").toLocal8Bit().constData(), libraryFolders);
        steamDirectoryList.append(steamFolder.filePath(""));
        QString pathString = "";

        for (auto kv : libraryFolders.get_child("LibraryFolders"))
        {
            if (std::isdigit(static_cast<int>(*kv.first.data())))
            {
                std::string path = kv.second.data();
                QDir dir(QString::fromStdString(path));
                if (dir.exists())
                {
                    steamDirectoryList.append(dir.filePath(""));
                    pathString += dir.filePath("");
                    pathString += "\n";
                }
            }
        }

        descLabel->setText(descLabel->text() + "\n\nLibrary folders:\n" + pathString);

        steamBox->setChecked(true);
        steamPath = steamFolder.filePath("");
    }
    else
    {
        statusLabel->setPixmap(QPixmap(":SystemMenu/Icons/Cross.svg"));
        descLabel = new QLabel("Steam not found. Verify installation and try again.");
    }
    layout->addWidget(platformLabel, 0, 0, 0);
    layout->addWidget(descLabel, 1, 0, 0);
    layout->addWidget(statusLabel, 0, 1, 0);
    layout->setRowMinimumHeight(1, 40);
}