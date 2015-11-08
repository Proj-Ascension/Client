#include "uplay_drm.h"
#include <QStandardPaths>
#include <QSettings>
#include <QLineEdit>
#include <QCheckBox>


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

QWidget* UplayDRM::createPane()
{
    int row = 0;
    for (pt::ptree::value_type& games : uplayTree.get_child("games"))
    {
        boost::optional<std::string> exeTest = games.second.get_optional<std::string>("exes");
        if (exeTest)
        {
            QButtonGroup* group = new QButtonGroup();
            QLineEdit* name = new QLineEdit(QString::fromStdString(games.second.get<std::string>("name")));
            name->setFixedWidth(350);
            QCheckBox* checkBox = new QCheckBox();
            layout->addWidget(name, row, 0, Qt::AlignVCenter | Qt::AlignLeft);
            row++;
            for (auto& exe : games.second.get_child("exes"))
            {
                checkBox = new QCheckBox("Executable: " + QString::fromStdString(exe.second.data()));
                group->addButton(checkBox);
                layout->addWidget(checkBox, row, 0, Qt::AlignVCenter | Qt::AlignLeft);
                row++;
            }
            buttonGroupVector.push_back(group);
        }
    }
    viewport->setLayout(layout);
    scrollArea->setWidget(viewport);

    return scrollArea;
}

void UplayDRM::findGames()
{
    rootDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList folderList = rootDir.entryList();
    QStringList ignoreList;
    ignoreList << "cache"
    << "data"
    << "locales"
    << "logs";
    int count = 0;

    for (auto i : folderList)
    {
        if (!ignoreList.contains(i))
        {
            pt::ptree& node = uplayTree.add("games.game", "");
            QDir dir(rootDir.absoluteFilePath(i));
            dir.setNameFilters(QStringList("*.exe"));
            dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
            QStringList test = recursiveFindFiles(dir);
            node.put("name", dir.dirName().toLocal8Bit().constData());
            for (auto exe : test)
            {
                node.add("exes.exe", exe.toLocal8Bit().constData());
            }
            count++;
        }
    }

    uplayTree.add("games.count", count);
}

pt::ptree UplayDRM::getGames()
{
    return uplayTree;
}

QList<QButtonGroup*> UplayDRM::getButtonGroupVector()
{
    return buttonGroupVector;
}
