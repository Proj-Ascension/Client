#include "steam_drm.h"
#include <QSettings>

/** Check if Steam is installed on the current computer, if applicable, and sets some values for later pages to
 * check on.
 * \see checkOriginExists()
 * \see checkUplayExists()
 */
void SteamDRM::checkSteamExists()
{
#if defined(__linux__)
    QProcess which;
    which.setProcessChannelMode(QProcess::MergedChannels);
    QStringList args;
    args << "steam";
    which.start("which", args);
    which.waitForFinished();
    if (which.exitCode() == 0)
    {
        this->setRootDir(QDir(QDir::homePath() + "/.local/share/Steam"));
    }
#elif defined(_WIN32) || defined(_WIN64)
    QSettings settings("HKEY_CURRENT_USER\\Software\\Valve\\Steam", QSettings::NativeFormat);
    if (!settings.value("SteamPath").isNull())
    {
        QDir steamTest = QDir(settings.value("SteamPath").toString()).canonicalPath();
        if (steamTest.filePath("").trimmed() != "" && steamTest.exists() && steamTest != QDir("."))
        {
            this->setRootDir(steamTest);
            this->setIsInstalled();
        }
    }
#elif defined(__APPLE__)
    this->setRootDir(QDir(QDir::home().filePath("Library/Application Support/Steam")));
#endif
}

QWidget SteamDRM::isExists()
{

}