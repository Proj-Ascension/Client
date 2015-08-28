#include <QtWidgets>
#include <iostream>
#include "Wizard.h"
#include "Libs/SteamVdfParse.hpp"
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>
#include <future>

namespace pt = boost::property_tree;

Wizard::Wizard(QWidget* parent, QString dbPath) : QWizard(parent), db(dbPath + "ascension.db")
{
    drmPage = new DRMPage();
    ResultsPage* resultsPage = new ResultsPage(db, *drmPage);
    setPage(pages::INTRO, new IntroPage());
    setPage(pages::DRM, drmPage);
    setPage(pages::RESULTS, resultsPage);
    setPage(pages::FINAL, new FinalPage());
    setWindowTitle("Project Ascension setup");
}

IntroPage::IntroPage(QWidget* parent) : QWizardPage(parent)
{
    setTitle("Finding games");
    setSubTitle("This wizard will try to find all your games.");
    auto layout = new QGridLayout();
    auto label = new QLabel("Click next to attempt to find Steam, Origin and Uplay games where applicable.");
    layout->addWidget(label);
    setLayout(layout);
}

FinalPage::FinalPage(QWidget* parent) : QWizardPage(parent)
{
    setTitle("Done");
}

DRMPage::DRMPage(QWidget* parent) : QWizardPage(parent)
{
    setTitle("Checking for Steam");
    steamBox = new QCheckBox();
    originBox = new QCheckBox();
    uplayBox = new QCheckBox();
    registerField("steamFound", steamBox);
    registerField("uplayFound", uplayBox);
    registerField("originFound", originBox);
    layout = new QGridLayout();
    platformLabel = new QLabel("<b>Steam</b>");
    descLabel = new QLabel();
    statusLabel = new QLabel();
    platformLabel->setTextFormat(Qt::TextFormat::RichText);
    checkSteamExists();

#if defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
    setTitle(title() += ", Origin");
    platformLabel = new QLabel("<b>Origin</b>");
    statusLabel = new QLabel();
    checkOriginExists();
#endif

#if defined(_WIN32) || defined (_WIN64)
    setTitle(title() += " and Uplay.");
    platformLabel = new QLabel("<b>Uplay</b>");
    statusLabel = new QLabel();
    checkUplayExists();
#endif

    setLayout(layout);
}

void DRMPage::checkOriginExists()
{
    QDir originRoot;
    QDir originFolder;
#if defined(_WIN32) || defined(_WIN64)
    originRoot = QDir(qgetenv("APPDATA").append("/Origin"));
#else
    originRoot = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation).append("/Origin/");
#endif


    if (originRoot.exists())
    {
        pt::ptree originTree;
        read_xml(originRoot.filePath("local.xml").toLocal8Bit().constData(), originTree);


        for (auto &xmlIter : originTree.get_child("Settings"))
        {
            if (xmlIter.second.get<std::string>("<xmlattr>.key") == "DownloadInPlaceDir")
            {
                originFolder = QString::fromStdString(xmlIter.second.get<std::string>("<xmlattr>.value"));
                break;
            }
        }

        if (originFolder == QDir("."))
        {
            originFolder = QDir("C:\\Program Files (x86)\\Origin Games\\");
        }
    }

    if (originFolder.filePath("").trimmed() != "" && originFolder.exists())
    {
        statusLabel->setPixmap(QPixmap(":/SystemMenu/Icons/Tick.svg"));
        descLabel = new QLabel("Origin found in " + originFolder.filePath(""));
        originBox->setChecked(true);
        originPath = originFolder.filePath("");
    }
    else
    {
        statusLabel->setPixmap(QPixmap(":/SystemMenu/Icons/Cross.svg"));
        descLabel = new QLabel("Origin not found on the system. Install and try again.");
    }
    layout->addWidget(platformLabel, 3, 0, 0);
    layout->addWidget(descLabel, 4, 0, 0);
    layout->addWidget(statusLabel, 3, 1, 0);
}

void DRMPage::checkSteamExists()
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

    if (steamFolder.filePath("").trimmed() != "" && steamFolder.exists())
    {
        statusLabel->setPixmap(QPixmap(":SystemMenu/Icons/Tick.svg"));
        descLabel = new QLabel("Steam root found in " + steamFolder.filePath(""));
        steamBox->setChecked(true);
        steamPath = steamFolder.filePath("");
    }
    else
    {
        statusLabel->setPixmap(QPixmap(":SystemMenu/Icons/Cross.svg"));
        descLabel = new QLabel("Steam not found, verify installation and try again.");
    }
    layout->addWidget(platformLabel, 0, 0, 0);
    layout->addWidget(descLabel, 1, 0, 0);
    layout->addWidget(statusLabel, 0, 1, 0);
}

void DRMPage::checkUplayExists()
{
    QDir uplayFolder;
    QDir uplayRoot = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation).append("/Ubisoft Game Launcher");
    if (uplayRoot.exists())
    {
        std::ifstream uplaySettings(uplayRoot.filePath("settings.yml").toLocal8Bit().constData(), std::ifstream::in);
        std::string line;
        if (uplaySettings.is_open())
        {
            while (std::getline(uplaySettings, line))
            {
                if (line.find("game_installation_path") != std::string::npos)
                {
                    std::vector<std::string> strSplit;
                    boost::split(strSplit, line, boost::is_any_of("\""));
                    uplayFolder = QDir(QString::fromStdString(strSplit.at(1)));
                    break;
                }
            }
        }

        if (uplayFolder == QDir("."))
        {
            QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Uplay", QSettings::NativeFormat);
            if (!settings.value("InstallLocation").isNull())
            {
                uplayFolder = QDir(settings.value("InstallLocation").toString());
            }
        }
    }

    if (uplayFolder.filePath("").trimmed() != "" && uplayFolder.exists())
    {
        statusLabel->setPixmap(QPixmap(":/SystemMenu/Icons/Tick.svg"));
        descLabel = new QLabel("Uplay found in " + uplayFolder.filePath(""));
        uplayBox->setChecked(true);
        uplayPath = uplayFolder.filePath("");
    }
    else
    {
        statusLabel->setPixmap(QPixmap(":/SystemMenu/Icons/Cross.svg"));
        descLabel = new QLabel("Uplay not found on the system. Install and try again.");
    }
    layout->addWidget(platformLabel, 5, 0, 0);
    layout->addWidget(descLabel, 6, 0, 0);
    layout->addWidget(statusLabel, 5, 1, 0);
}

ResultsPage::ResultsPage(Database db, DRMPage& drmPage, QWidget* parent) : QWizardPage(parent), db(db)
{
    setSubTitle("We found the following on your system.");
    steamRoot = drmPage.steamPath;
    originRoot = drmPage.originPath;
    uplayRoot = drmPage.uplayPath;
}

void ResultsPage::initializePage()
{
    if (field("steamFound").toBool())
    {
        auto t = std::async(&ResultsPage::findSteamGames, this);
        t.get();
    }

    if (field("originFound").toBool())
    {
        auto t = std::async(&ResultsPage::findOriginGames, this);
        t.get();
    }

    if (field("uplayFound").toBool())
    {
        auto t = std::async(&ResultsPage::findUplayGames, this);
        t.get();
    }
    setTitle(QString("We found ") + QString::number(steamVector.size()) + QString(" Steam game") + (steamVector.size() >= 2 ? QString("s."):QString(".")));
    top_layout = new QGridLayout();
    layout = new QGridLayout();
    scrollArea = new QScrollArea();

    for (auto i : steamVector)
    {
        QLabel* name = new QLabel("<b>" + i.at(0) +"</b>");
        QLabel* exe = new QLabel(i.at(2).remove(steamRoot.filePath("")));
        exe->setText(exe->text().remove(0, 1));
        name->setTextFormat(Qt::TextFormat::RichText);
        layout->addWidget(name);
        layout->addWidget(exe);
    }

    QWidget* viewport = new QWidget();
    viewport->setLayout(layout);
    scrollArea->setWidget(viewport);
    top_layout->addWidget(scrollArea);
    setLayout(top_layout);
}

void ResultsPage::findSteamGames()
{
    QDir steamAppsDir = steamRoot.filePath("steamapps");
    if (!steamAppsDir.exists())
    {
        steamAppsDir = steamRoot.filePath("SteamApps");
    }
    pt::ptree libraryFolders;
    pt::read_info(steamAppsDir.filePath("libraryfolders.vdf").toLocal8Bit().constData(), libraryFolders);
    steamDirectoryList.append(steamRoot.filePath(""));
    QString pathString = "" + steamDirectoryList.at(0) + "\n";

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

    // TODO: Make this prompting better/less obtrusive
//    bool directoryPlural = (steamDirectoryList.size() > 1);
//    int ret = QMessageBox(QMessageBox::Question, "Found " + QString::number(steamDirectoryList.size()) + " director" + (directoryPlural ? "ies" : "y"), QString::number(steamDirectoryList.size()) + " directories have been found.\n\n" + pathString + "Proceed?", QMessageBox::Yes | QMessageBox::No).exec();
//    switch (ret)
//    {
//        case QMessageBox::Yes:
            parseAcf(steamRoot);
//            break;
//        case QMessageBox::No:
//            break;
//        default:
//            break;
//    }
}

void ResultsPage::findOriginGames()
{
    QStringList ignoreList;
    ignoreList << "Cleanup.exe"
               << "Touchup.exe"
               << "DXSETUP.exe"
               << "vcredist_x86.exe"
               << "vcredist_x64.exe"
               << "ActivationUI.exe"
               << "PatchProgress.exe"
               << "activation.exe"
               << "EACoreServer.exe"
               << "EAProxyInstaller.exe"
               << "D3D11Install.exe"
               << "ATISET.EXE"
               << "DOS4GW.EXE"
               << "INFO.EXE"
               << "INSTALL.EXE"
               << "INTRO.EXE"
               << "MUNGE.EXE"
               << "SC2K.EXE"
               << "UNIVBE.EXE"
               << "VDETECT.EXE"
               << "VRF_DLL.EXE"
               << "WILLTV.EXE";
    originRoot.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList folderList = originRoot.entryList();
    QHash<QString, QStringList> masterList;
    for (auto i : folderList)
    {
        // TODO: Populate a widget with this info
        QDir dir(originRoot.absoluteFilePath(i));
        dir.setNameFilters(QStringList("*.exe"));
        dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        qDebug() << "Looking in: " << dir.filePath("");
        QStringList test = recursiveFindFiles(dir, ignoreList);
        masterList.insert(dir.filePath(""), test);
    }

    QHashIterator<QString, QStringList> masterIter(masterList);
    while (masterIter.hasNext())
    {
        masterIter.next();
        qDebug() << "Found in: " << masterIter.key();
        for (auto fileIter : masterIter.value())
        {
            qDebug() << fileIter;
        }
    }
}

void ResultsPage::findUplayGames()
{
    QStringList ignoreList;
    ignoreList << "vista_update32.exe"
               << "vista_update64.exe"
               << "vcredist_x86.exe"
               << "vcredist_x86_2008SP1.exe"
               << "vcredist_x86_sp1.exe"
               << "firewallGE.exe"
               << "firewallGE_rem.exe"
               << "GDFInstall.exe"
               << "GDFTool.exe"
               << "DXSETUP.exe"
               << "uplay_unins.exe"
               << "dotNetFx40_Client_x86_x64.exe"
               << "ACBF_cleanup.exe";
    uplayRoot.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList folderList = uplayRoot.entryList();
    QHash<QString, QStringList> masterList;
    for (auto i : folderList)
    {
        // TODO: Populate a widget with this info
        QDir dir(uplayRoot.absoluteFilePath(i));
        // dir.setNameFilters(QStringList("*.exe"));
        dir.setFilter(QDir::Files | QDir::Executable | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        qDebug() << "Looking in: " << dir.filePath("");
        QStringList test = recursiveFindFiles(dir, ignoreList);
        masterList.insert(dir.filePath(""), test);
    }

    QHashIterator<QString, QStringList> masterIter(masterList);
    while (masterIter.hasNext())
    {
        masterIter.next();
        qDebug() << "Found in: " << masterIter.key();
        for (auto fileIter : masterIter.value())
        {
            qDebug() << fileIter;
        }
    }
}

void ResultsPage::parseAcf(QDir steamRoot)
{
    // TODO: This stuff needs its own thread
    QString vdfPath = steamRoot.filePath("appcache/appinfo.vdf");
    qDebug() << "Parsing Steam vdf, located at:" << vdfPath;
    auto games = SteamVdfParse::parseVdf(vdfPath.toLocal8Bit().constData());

    for (QString iter : steamDirectoryList)
    {
        QDir steamAppsDir(iter);
        if (steamAppsDir.exists("SteamApps"))
        {
            steamAppsDir = steamAppsDir.filePath("SteamApps");
        }
        else
        {
            steamAppsDir = steamAppsDir.filePath("steamapps");
        }

        QStringList fileList = steamAppsDir.entryList(QStringList("*.acf"), QDir::Files | QDir::NoSymLinks);

        for (auto fileIter : fileList)
        {
            boost::property_tree::ptree fileTree;
            std::string acfDir = steamAppsDir.filePath(fileIter).toLocal8Bit().constData();
            boost::property_tree::info_parser::read_info(acfDir, fileTree);

            QString name;
            try
            {
                name = QString::fromStdString(fileTree.get<std::string>("AppState.name"));
            }
            catch (std::exception& e)
            {
                if (e.what() == "No such node")
                {
                    name = QString::fromStdString(fileTree.get<std::string>("AppState.UserConfig.name"));
                }
            }

            // TODO: Either add SteamID to db, or add getGameByPath

            if (!std::get<0>(db.isExistant(name)))
            {
                QString path = steamAppsDir.filePath("common/" + QString::fromStdString(fileTree.get<std::string>("AppState.installdir")));
                QString exe;
                QString args;

                int id;
                try
                {
                    id = stoi(fileTree.get<std::string>("AppState.appID"));
                }
                catch (std::exception& e)
                {
                    id = stoi(fileTree.get<std::string>("AppState.appid"));
                }

                try
                {
                    auto game = games.at(id);
                    auto launch = game.pt.get_child("config.launch");

                    // Loop through the 0, 1, and 2 configurations
                    for (auto pair : launch)
                    {
                        boost::property_tree::ptree section = pair.second;

                        QString oslist = QString::fromStdString(section.get("config.oslist", "windows"));

#if defined(__linux__)
                        if (oslist == "linux")

#elif defined(_WIN32) || defined(_WIN64)
                            if (oslist == "windows")
#elif defined(__APPLE__)
                        if (oslist == "macos")
#endif
                        {
                            exe = QDir(path).filePath(QString::fromStdString(section.get<std::string>("executable")));
                            exe = QString(QDir::cleanPath(exe));
                            path = QDir(path).filePath(QString::fromStdString(section.get("workingdir", "")));
                            path = QString(QDir::cleanPath(path));
                            args = QString::fromStdString(section.get("arguments", ""));
                        }
                    }
                }
                catch (const std::out_of_range&)
                {
                    qDebug() << "The game id:" << id << "was not found in the Steam appinfo.vdf.";
                    QStringList exeList = QDir(path).entryList(QDir::Files | QDir::NoSymLinks | QDir::Executable);

                    QFileDialog exeDialog;
                    exeDialog.setWindowTitle("Select Executable");
                    exeDialog.setFileMode(QFileDialog::ExistingFile);
                    exeDialog.setDirectory(path);
                    if (exeDialog.exec())
                    {
                        exe = exeDialog.selectedFiles().at(0);
                    }
                }

//                db.addGame(name, path, exe, args);
//                refreshGames();
                steamVector.push_back(std::vector<QString>{name, path, exe, args});
            }
        }
    }
}

QStringList ResultsPage::recursiveFindFiles(QDir dir, QStringList ignoreList)
{
    QStringList dirList;
    QDirIterator it(dir, QDirIterator::Subdirectories);

    while (it.hasNext())
    {
        QDir cur = it.next();
        if (!ignoreList.contains(cur.dirName()))
        {
            bool found = false;
            for (auto foundIter : dirList)
            {
                if (QDir(foundIter).dirName() == cur.dirName())
                {
                    found = true;
                    break;
                }
            }
            if (!found && cur.dirName() != QDir::currentPath())
            {
                dirList.append(cur.filePath(""));
            }
        }
    }

    return dirList;
}
