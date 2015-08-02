#include "Library.h"
#include "ui_Library.h"
#include "Libs/SteamVdfParse.hpp"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include <QStandardPaths>
#include <QDirIterator>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/algorithm/string.hpp>

#include <cctype>

#if defined(_WIN32) || defined(_WIN64)
#include <QSettings>
#endif

namespace pt = boost::property_tree;

Library::Library(Database db)
    : QWidget(0),
      db(db),
      ui(new Ui::Library),
      runningProcess(new QProcess(this))
{
    ui->setupUi(this);
    this->setObjectName("libraryUI");
    connect(runningProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finished(int, QProcess::ExitStatus)));
    connect(runningProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(onLaunchError(QProcess::ProcessError)));

    QList<Game> games = db.getGames();
    for (auto game : games)
    {
        qDebug() << game.id << game.gameName << game.gameDirectory << game.executablePath;
    }

    refreshGames();

    // For debugging
    bool loadSteam = true;
    bool loadOrigin = false;
    bool loadUplay = false;
    QDir originRoot(qgetenv("APPDATA").append("/Origin"));
    if (originRoot.exists() && loadOrigin)
    {
        findOriginGames(originRoot);
    }
    else
    {
        qDebug() << "Origin not found. Possibly not installed.";
    }

    QDir steamRoot;
#if defined(_WIN32) || defined(_WIN64)
    QSettings settings("HKEY_CURRENT_USER\\Software\\Valve\\Steam", QSettings::NativeFormat);
    if (!settings.value("SteamPath").isNull())
    {
        steamRoot = QDir(settings.value("SteamPath").toString());
    }

#elif defined(__APPLE__)
    // TODO: however OS X handles steam
    return;
#elif defined(__linux__)
    steamRoot = QDir(QDir::home().filePath(".steam/steam"));
#else
    QMessageBox(QMessageBox::Critical, "Error", "Platform doesn't support steam.");
    return;
#endif

    if (steamRoot != QDir(".") && loadSteam)
    {
        findSteamGames(steamRoot);
    }
    else
    {
        qDebug("Steam was not found, probably not installed.");
    }

    QDir uplayRoot = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation).append("/Ubisoft Game Launcher");
    if (uplayRoot.exists() && loadUplay)
    {
        findUplayGames(uplayRoot);
    }
    else
    {
        qDebug("UPlay was not found, probably not installed.");
    }
}

Library::~Library()
{
    delete ui;
    delete runningProcess;
}

void Library::on_testLaunch_clicked()
{
    if (!isProcessRunning())
    {
        auto selection = ui->gameListWidget->currentItem();
        if (selection != nullptr)
        {
            Game game = db.getGameByName(selection->text());
            runProcess(game.executablePath, game.gameDirectory);
        }
    }
    else
    {
        QMessageBox messageBox;
        messageBox.setText("Error: an application is already running.");
        messageBox.exec();
    }
}

void Library::on_addGame_clicked()
{
    QString name = QInputDialog::getText(0, "Game Name", "Game Name:");

    if (name.trimmed() == "")
    {
        QMessageBox::critical(0, "Error", "You must specify a game name!");
        return;
    }

    QFileDialog exeDialog;
    exeDialog.setWindowTitle("Select Executable");
    exeDialog.setFileMode(QFileDialog::ExistingFile);
#if defined(__unix__)
    exeDialog.setDirectory(QDir::home());
#elif defined(_WIN32) || defined(_WIN64)
    exeDialog.setDirectory("C:");
#endif

    if (exeDialog.exec())
    {
        QStringList files = exeDialog.selectedFiles();
        QString exe = files.at(0);
#ifdef Q_WS_MACX
        //Get the binary from the app bundle
        QDir dir(file + "/Contents/MacOS");
        // TODO: Change to dir.entryList(QDir::NoDotAndDotDot) to be safe
        QStringList fileList = dir.entryList();
        file = dir.absoluteFilePath(fileList.at(2));  // USUALLY this is the executable (after ., ..)
#endif

        QFileDialog wdDialog;  // Working Directory
        wdDialog.setWindowTitle("Select Working Directory");
        wdDialog.setFileMode(QFileDialog::DirectoryOnly);
        wdDialog.setDirectory(exeDialog.directory().absolutePath());

        if (wdDialog.exec())
        {
            QStringList dirs = wdDialog.selectedFiles();
            QString dir = dirs.at(0);

            qDebug() << "Adding game:" << name << exe << dir;
            db.addGame(name, dir, exe);

            refreshGames();
        }
    }
}

void Library::on_removeGame_clicked()
{
    auto selection = ui->gameListWidget->currentItem();
    if (selection != nullptr)
    {
        db.removeGameByName(selection->text());
        refreshGames();
    }
}

void Library::runProcess(QString file, QString workingDirectory)
{
    // TODO: Implement some threading
    if (!isProcessRunning())
    {
        qDebug() << "Launching:" << file << ", at" << workingDirectory;
        runningProcess->setWorkingDirectory(workingDirectory);
        runningProcess->setStandardErrorFile("error.txt");
        runningProcess->setStandardOutputFile("log.txt");
        runningProcess->start(file, QStringList());
        runningProcess->waitForStarted();
    }
}

void Library::refreshGames()
{
    ui->gameListWidget->clear();
    QList<Game> gameList = db.getGames();
    for (auto game : gameList)
    {
        ui->gameListWidget->addItem(game.gameName);
    }
}

void Library::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitCode != 0)
    {
        QMessageBox(QMessageBox::Warning, "Warning", "The game finished, but it claims to have encountered an error").exec();
    }
}

void Library::onLaunchError(QProcess::ProcessError error)
{
    switch (error)
    {
        case QProcess::FailedToStart:
            QMessageBox(QMessageBox::Critical, "Error", "Could not start the game. Please double check that you are using the correct file to launch it.").exec();
            break;
        case QProcess::Crashed:
            QMessageBox(QMessageBox::Warning, "Crash!", "The launched game has crashed").exec();
            break;
        default:
            // Other cases are errors unrelated to startup, so let's not handle them
            break;
    }
}

bool Library::isProcessRunning() const
{
    // We shall consider "Starting" to be running here too
    return runningProcess->state() != QProcess::NotRunning;
}

void Library::findSteamGames(QDir steamRoot)
{
    QDir steamAppsDir = steamRoot.filePath("steamapps");
    if (!steamAppsDir.exists())
    {
        steamAppsDir = steamRoot.filePath("SteamApps");
    }
    pt::ptree libraryFolders;
    pt::read_info(steamAppsDir.filePath("libraryfolders.vdf")
                      .toLocal8Bit()
                      .constData(),
                  libraryFolders);
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
    bool directoryPlural = (steamDirectoryList.size() > 1);
    int ret = QMessageBox(QMessageBox::Question,
                          "Found " + QString::number(steamDirectoryList.size()) +
                              " director" + (directoryPlural ? "ies" : "y"),
                          QString::number(steamDirectoryList.size()) +
                              " directories have been found.\n\n" + pathString +
                              "Proceed?",
                          QMessageBox::Yes | QMessageBox::No)
                  .exec();
    switch (ret)
    {
        case QMessageBox::Yes:
            parseAcf(steamRoot);
            break;
        case QMessageBox::No:
            break;
        default:
            break;
    }
}

void Library::findOriginGames(QDir originRoot)
{
    QDir originFolder;
    pt::ptree originTree;
    read_xml(originRoot.filePath("local.xml").toLocal8Bit().constData(), originTree);

    for (auto& xmlIter : originTree.get_child("Settings"))
    {
        if (xmlIter.second.get<std::string>("<xmlattr>.key") == "DownloadInPlaceDir")
        {
            originFolder = QString::fromStdString(xmlIter.second.get<std::string>("<xmlattr>.value"));
            qDebug() << originFolder;
            break;
        }
    }

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
               << "D3D11Install.exe";
    originFolder.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList folderList = originFolder.entryList();
    QHash<QString, QStringList> masterList;
    for (auto i : folderList)
    {
        // TODO: Populate a widget with this info
        QDir dir(originFolder.absoluteFilePath(i));
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

void Library::findUplayGames(QDir uplayRoot)
{
    QDir uplayFolder;
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
            }
        }
    }

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
    uplayFolder.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList folderList = uplayFolder.entryList();
    QHash<QString, QStringList> masterList;
    for (auto i : folderList)
    {
        // TODO: Populate a widget with this info
        QDir dir(uplayFolder.absoluteFilePath(i));
        //dir.setNameFilters(QStringList("*.exe"));
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

QStringList Library::recursiveFindFiles(QDir dir, QStringList ignoreList)
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

void Library::parseAcf(QDir steamRoot)
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
            pt::ptree fileTree;
            std::string acfDir = steamAppsDir.filePath(fileIter).toLocal8Bit().constData();
            pt::read_info(acfDir, fileTree);

            QString name;
            try
            {
                name = QString::fromStdString(fileTree.get<std::string>("AppState.name"));
            }
            catch (std::exception& e)
            {
                if (e.what() == "No such node")
                    name = QString::fromStdString(fileTree.get<std::string>("AppState.UserConfig.name"));
            }

            // TODO: Either add SteamID to db, or add getGameByPath
            if (!std::get<0>(db.isExistant(name)))
            {
                QString path = steamAppsDir.filePath("common/" + QString::fromStdString(fileTree.get<std::string>("AppState.installdir")));

                QString exe;
                int id;
                try
                {
                    id = std::stoi(fileTree.get<std::string>("AppState.appID"));
                }
                catch (std::exception& e)
                {
                    id = std::stoi(fileTree.get<std::string>("AppState.appid"));
                }
#if defined(_WIN32) || defined(_WIN64)
                auto osNum = 0;
#elif defined(__APPLE__)
                auto osNum = 1;
#elif defined(__linux__)
                auto osNum = 2;
#endif
                try
                {
                    auto game = games.at(id);
                    auto config = game.sections.at("config");
                    auto launch = config.get<SteamVdfParse::Section>("launch");

                    for (auto pair : launch.kv)
                    {
                        auto section = boost::any_cast<SteamVdfParse::Section>(pair.second);

                        exe = QDir(path).filePath(QString::fromStdString(section.get<std::string>("executable")));

                        try
                        {
                            path = QDir(path).filePath(QString::fromStdString(section.get<std::string>("workingdir")));
                        }
                        catch (boost::bad_any_cast&)
                        {
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

                db.addGame(name, path, exe);
                refreshGames();
            }
        }
    }
}