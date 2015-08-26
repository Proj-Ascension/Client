#include "Library.h"
#include "ui_Library.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include <QStandardPaths>
#include <QDirIterator>
#include <QSettings>

#include <boost/algorithm/string.hpp>

/** Library constructor
 * Initialize the UI and generate an initial list of all the games available.
*/
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
    bool loadOrigin = true;
    bool loadUplay = true;
    QDir originRoot;
#if defined(_WIN32) || defined(_WIN64)
    originRoot = QDir(qgetenv("APPDATA").append("/Origin"));
#elif defined(__APPLE__)
    originRoot = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation).append("/Origin/");
#else
    QMessageBox(QMessageBox::Critical, "Error", "Platform does not support Origin.");
    return;
#endif

#if defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
    if (originRoot.exists() && loadOrigin)
    {
//        findOriginGames(originRoot);
    }
    else
    {
        qDebug() << "Origin not found. Possibly not installed.";
    }
#endif

    QDir steamRoot;
#if defined(_WIN32) || defined(_WIN64)
    QSettings settings("HKEY_CURRENT_USER\\Software\\Valve\\Steam", QSettings::NativeFormat);
    if (!settings.value("SteamPath").isNull())
    {
        steamRoot = QDir(settings.value("SteamPath").toString());
    }
#elif defined(__APPLE__)
    steamRoot = QDir(QDir::home().filePath("Library/Application Support/Steam"));
#elif defined(__linux__)
    steamRoot = QDir(QDir::home().filePath(".steam/steam"));
#else
    QMessageBox(QMessageBox::Critical, "Error", "Platform doesn't support steam.");
    return;
#endif

    if (steamRoot != QDir(".") && loadSteam)
    {
//        findSteamGames(steamRoot);
    }
    else
    {
        qDebug("Steam was not found, probably not installed.");
    }

#if defined(_WIN32) || defined(_WIN64)
    QDir uplayRoot = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation).append("/Ubisoft Game Launcher");
    if (uplayRoot.exists() && loadUplay)
    {
//        findUplayGames(uplayRoot);
    }
    else
    {
        qDebug("UPlay was not found, probably not installed.");
    }
#endif
}

Library::~Library()
{
    delete ui;
    delete runningProcess;
}

/** Event handler for launching a game.
 * Populates a message box on failure, or runs the games upon success.
*/
void Library::on_testLaunch_clicked()
{
    if (!isProcessRunning())
    {
        auto selection = ui->gameListWidget->currentItem();
        if (selection != nullptr)
        {
            Game game = db.getGameByName(selection->text());
            if (game.arguments.trimmed() == "")
            {
                runProcess(game.executablePath, game.gameDirectory);
            }
            else
            {
                runProcessWithArgs(game.executablePath, game.gameDirectory, game.arguments);
            }
        }
    }
    else
    {
        QMessageBox messageBox;
        messageBox.setText("Error: an application is already running.");
        messageBox.exec();
    }
}

/** Event handler for adding a game.
 * Prompts the user for various paths, and adds the final game to the database.
*/
void Library::on_addGame_clicked()
{
    QString name = QInputDialog::getText(0, "Game Name", "Game Name:");

    QString args = QInputDialog::getText(0, "Arguments for " + name, "Args (optional): ");

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
        // Get the binary from the app bundle
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

            qDebug() << "Adding game:" << name << exe << dir << args;
            db.addGame(name, dir, exe, args);

            refreshGames();
        }
    }
}

/** Event handler for removing a game.
*/
void Library::on_removeGame_clicked()
{
    auto selection = ui->gameListWidget->currentItem();
    if (selection != nullptr)
    {
        db.removeGameByName(selection->text());
        refreshGames();
    }
}

/** Launch a new QProcess using the passed exe and working directory.
 * \param file Location of the exe to run
 * \param workingDirectory The directory that QProcess should spawn in
*/
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

/** Launch a new QProcess using the passed exe and working directory.
 * \param file Location of the exe to run
 * \param workingDirectory The directory that QProcess should spawn in
 * \param args String of arguments to launch the executable with
*/
void Library::runProcessWithArgs(QString file, QString workingDirectory, QString args)
{
    // TODO: Implement some threading
    if (!isProcessRunning())
    {
        qDebug() << "Launching:" << file << ", at" << workingDirectory << "with " << args;
        runningProcess->setWorkingDirectory(workingDirectory);
        runningProcess->setStandardErrorFile("error.txt");
        runningProcess->setStandardOutputFile("log.txt");
        runningProcess->start(file, QStringList(args.split(" ")));
        runningProcess->waitForStarted();
    }
}

/** Recreate the list of games displayed in the main widget.
*/
void Library::refreshGames()
{
    ui->gameListWidget->clear();
    QList<Game> gameList = db.getGames();
    for (auto game : gameList)
    {
        ui->gameListWidget->addItem(game.gameName);
    }
}

/** Attempt to handle process ending unexpectedly or fork.
 * \param exitCode Exit code to check
 * \param exitStatus Status to check
*/
void Library::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitCode != 0)
    {
        QMessageBox(QMessageBox::Warning, "Warning", "The game finished, but it claims to have encountered an error").exec();
    }
}

/** Handle errors before the process has launched.
 * \param error The error to translate
*/
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

/** Check if a process is running already
 * \return Success/failure upon completion.
*/
bool Library::isProcessRunning() const
{
    // We shall consider "Starting" to be running here too
    return runningProcess->state() != QProcess::NotRunning;
}

/** Find the location of every steam game, using steamRoot as a basepoint.
 * \param steamRoot The root of your steam installation
*/
//void Library::findSteamGames(QDir steamRoot)
//{
//    QDir steamAppsDir = steamRoot.filePath("steamapps");
//    if (!steamAppsDir.exists())
//    {
//        steamAppsDir = steamRoot.filePath("SteamApps");
//    }
//    pt::ptree libraryFolders;
//    pt::read_info(steamAppsDir.filePath("libraryfolders.vdf").toLocal8Bit().constData(), libraryFolders);
//    steamDirectoryList.append(steamRoot.filePath(""));
//    QString pathString = "" + steamDirectoryList.at(0) + "\n";
//
//    for (auto kv : libraryFolders.get_child("LibraryFolders"))
//    {
//        if (std::isdigit(static_cast<int>(*kv.first.data())))
//        {
//            std::string path = kv.second.data();
//            QDir dir(QString::fromStdString(path));
//            if (dir.exists())
//            {
//                steamDirectoryList.append(dir.filePath(""));
//                pathString += dir.filePath("");
//                pathString += "\n";
//            }
//        }
//    }
//
//    // TODO: Make this prompting better/less obtrusive
//    bool directoryPlural = (steamDirectoryList.size() > 1);
//    int ret = QMessageBox(QMessageBox::Question, "Found " + QString::number(steamDirectoryList.size()) + " director" + (directoryPlural ? "ies" : "y"), QString::number(steamDirectoryList.size()) + " directories have been found.\n\n" + pathString + "Proceed?", QMessageBox::Yes | QMessageBox::No).exec();
//    switch (ret)
//    {
//        case QMessageBox::Yes:
//            parseAcf(steamRoot);
//            break;
//        case QMessageBox::No:
//            break;
//        default:
//            break;
//    }
//}

/** Find the location of every origin game as best we can. Unlike Valve, EA
 * decided to be almost as awkward as Ubisoft so this is mostly trial and error.
 * \param originRoot The root directory of you Origin installation
 */
//void Library::findOriginGames(QDir originRoot)
//{
//    QDir originFolder;
//    pt::ptree originTree;
//    read_xml(originRoot.filePath("local.xml").toLocal8Bit().constData(), originTree);
//
//
//    for (auto& xmlIter : originTree.get_child("Settings"))
//    {
//        if (xmlIter.second.get<std::string>("<xmlattr>.key") == "DownloadInPlaceDir")
//        {
//            originFolder = QString::fromStdString(xmlIter.second.get<std::string>("<xmlattr>.value"));
//            break;
//        }
//    }
//
//    if (originFolder == QDir("."))
//    {
//        originFolder = QDir("C:\\Program Files (x86)\\Origin Games\\");
//    }
//
//
//    QStringList ignoreList;
//    ignoreList << "Cleanup.exe"
//               << "Touchup.exe"
//               << "DXSETUP.exe"
//               << "vcredist_x86.exe"
//               << "vcredist_x64.exe"
//               << "ActivationUI.exe"
//               << "PatchProgress.exe"
//               << "activation.exe"
//               << "EACoreServer.exe"
//               << "EAProxyInstaller.exe"
//               << "D3D11Install.exe"
//               << "ATISET.EXE"
//               << "DOS4GW.EXE"
//               << "INFO.EXE"
//               << "INSTALL.EXE"
//               << "INTRO.EXE"
//               << "MUNGE.EXE"
//               << "SC2K.EXE"
//               << "UNIVBE.EXE"
//               << "VDETECT.EXE"
//               << "VRF_DLL.EXE"
//               << "WILLTV.EXE";
//    originFolder.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
//    QStringList folderList = originFolder.entryList();
//    QHash<QString, QStringList> masterList;
//    for (auto i : folderList)
//    {
//        // TODO: Populate a widget with this info
//        QDir dir(originFolder.absoluteFilePath(i));
//        dir.setNameFilters(QStringList("*.exe"));
//        dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
//        qDebug() << "Looking in: " << dir.filePath("");
//        QStringList test = recursiveFindFiles(dir, ignoreList);
//        masterList.insert(dir.filePath(""), test);
//    }
//
//    QHashIterator<QString, QStringList> masterIter(masterList);
//    while (masterIter.hasNext())
//    {
//        masterIter.next();
//        qDebug() << "Found in: " << masterIter.key();
//        for (auto fileIter : masterIter.value())
//        {
//            qDebug() << fileIter;
//        }
//    }
//}
//
///** Find the location of every Uplay game as best we can. Ubisoft also decided
// * to be as awkward as possible, so this isn't accurate.
// * \param uplayRoot The root directory of your Uplay installation.
//*/
//void Library::findUplayGames(QDir uplayRoot)
//{
//    QDir uplayFolder;
//    std::ifstream uplaySettings(uplayRoot.filePath("settings.yml").toLocal8Bit().constData(), std::ifstream::in);
//    std::string line;
//    if (uplaySettings.is_open())
//    {
//        while (std::getline(uplaySettings, line))
//        {
//            if (line.find("game_installation_path") != std::string::npos)
//            {
//                std::vector<std::string> strSplit;
//                boost::split(strSplit, line, boost::is_any_of("\""));
//                uplayFolder = QDir(QString::fromStdString(strSplit.at(1)));
//                break;
//            }
//        }
//    }
//
//    if (uplayFolder == QDir("."))
//    {
//        QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Uplay", QSettings::NativeFormat);
//        if (!settings.value("InstallLocation").isNull()) {
//            uplayFolder = QDir(settings.value("InstallLocation").toString());
//        }
//    }
//
//    QStringList ignoreList;
//    ignoreList << "vista_update32.exe"
//               << "vista_update64.exe"
//               << "vcredist_x86.exe"
//               << "vcredist_x86_2008SP1.exe"
//               << "vcredist_x86_sp1.exe"
//               << "firewallGE.exe"
//               << "firewallGE_rem.exe"
//               << "GDFInstall.exe"
//               << "GDFTool.exe"
//               << "DXSETUP.exe"
//               << "uplay_unins.exe"
//               << "dotNetFx40_Client_x86_x64.exe"
//               << "ACBF_cleanup.exe";
//    uplayFolder.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
//    QStringList folderList = uplayFolder.entryList();
//    QHash<QString, QStringList> masterList;
//    for (auto i : folderList)
//    {
//        // TODO: Populate a widget with this info
//        QDir dir(uplayFolder.absoluteFilePath(i));
//        // dir.setNameFilters(QStringList("*.exe"));
//        dir.setFilter(QDir::Files | QDir::Executable | QDir::NoDotAndDotDot | QDir::NoSymLinks);
//        qDebug() << "Looking in: " << dir.filePath("");
//        QStringList test = recursiveFindFiles(dir, ignoreList);
//        masterList.insert(dir.filePath(""), test);
//    }
//
//    QHashIterator<QString, QStringList> masterIter(masterList);
//    while (masterIter.hasNext())
//    {
//        masterIter.next();
//        qDebug() << "Found in: " << masterIter.key();
//        for (auto fileIter : masterIter.value())
//        {
//            qDebug() << fileIter;
//        }
//    }
//}
//
///** Given a directory, recursively find every file not part of the ignoreList.
// * \param dir The directory to search in.
// * \param ignoreList List of filenames to not add to the final list.
// * \return A QStringList of all the correct files.
//*/
//QStringList Library::recursiveFindFiles(QDir dir, QStringList ignoreList)
//{
//    QStringList dirList;
//    QDirIterator it(dir, QDirIterator::Subdirectories);
//
//    while (it.hasNext())
//    {
//        QDir cur = it.next();
//        if (!ignoreList.contains(cur.dirName()))
//        {
//            bool found = false;
//            for (auto foundIter : dirList)
//            {
//                if (QDir(foundIter).dirName() == cur.dirName())
//                {
//                    found = true;
//                    break;
//                }
//            }
//            if (!found && cur.dirName() != QDir::currentPath())
//            {
//                dirList.append(cur.filePath(""));
//            }
//        }
//    }
//
//    return dirList;
//}
//
///** Given the root of a Steam installation directory, for every appmanifest we
// * find, parse the correct information out of it.
// * \param steamRoot The root of a Steam installation.
//*/
//void Library::parseAcf(QDir steamRoot)
//{
//    // TODO: This stuff needs its own thread
//    QString vdfPath = steamRoot.filePath("appcache/appinfo.vdf");
//    qDebug() << "Parsing Steam vdf, located at:" << vdfPath;
//    auto games = SteamVdfParse::parseVdf(vdfPath.toLocal8Bit().constData());
//
//    for (QString iter : steamDirectoryList)
//    {
//        QDir steamAppsDir(iter);
//        if (steamAppsDir.exists("SteamApps"))
//        {
//            steamAppsDir = steamAppsDir.filePath("SteamApps");
//        }
//        else
//        {
//            steamAppsDir = steamAppsDir.filePath("steamapps");
//        }
//
//        QStringList fileList = steamAppsDir.entryList(QStringList("*.acf"), QDir::Files | QDir::NoSymLinks);
//
//        for (auto fileIter : fileList)
//        {
//            pt::ptree fileTree;
//            std::string acfDir = steamAppsDir.filePath(fileIter).toLocal8Bit().constData();
//            pt::read_info(acfDir, fileTree);
//
//            QString name;
//            try
//            {
//                name = QString::fromStdString(fileTree.get<std::string>("AppState.name"));
//            }
//            catch (std::exception& e)
//            {
//                if (e.what() == "No such node")
//                {
//                    name = QString::fromStdString(fileTree.get<std::string>("AppState.UserConfig.name"));
//                }
//            }
//
//            // TODO: Either add SteamID to db, or add getGameByPath
//            if (!std::get<0>(db.isExistant(name)))
//            {
//                QString path = steamAppsDir.filePath("common/" + QString::fromStdString(fileTree.get<std::string>("AppState.installdir")));
//                QString exe;
//                QString args;
//
//                int id;
//                try
//                {
//                    id = std::stoi(fileTree.get<std::string>("AppState.appID"));
//                }
//                catch (std::exception& e)
//                {
//                    id = std::stoi(fileTree.get<std::string>("AppState.appid"));
//                }
//
//                try
//                {
//                    auto game = games.at(id);
//                    auto launch = game.pt.get_child("config.launch");
//
//                    // Loop through the 0, 1, and 2 configurations
//                    for (auto pair : launch)
//                    {
//                        pt::ptree section = pair.second;
//
//                        QString oslist = QString::fromStdString(section.get("config.oslist", "windows"));
//
//#if defined(__linux__)
//                        if (oslist == "linux")
//
//#elif defined(_WIN32) || defined(_WIN64)
//                        if (oslist == "windows")
//#elif defined(__APPLE__)
//                        if (oslist == "macos")
//#endif
//                        {
//                            exe = QDir(path).filePath(QString::fromStdString(section.get<std::string>("executable")));
//                            path = QDir(path).filePath(QString::fromStdString(section.get("workingdir", "")));
//                            args = QString::fromStdString(section.get("arguments", ""));
//                        }
//                    }
//                }
//                catch (const std::out_of_range&)
//                {
//                    qDebug() << "The game id:" << id << "was not found in the Steam appinfo.vdf.";
//                    QStringList exeList = QDir(path).entryList(QDir::Files | QDir::NoSymLinks | QDir::Executable);
//
//                    QFileDialog exeDialog;
//                    exeDialog.setWindowTitle("Select Executable");
//                    exeDialog.setFileMode(QFileDialog::ExistingFile);
//                    exeDialog.setDirectory(path);
//                    if (exeDialog.exec())
//                    {
//                        exe = exeDialog.selectedFiles().at(0);
//                    }
//                }
//
//                db.addGame(name, path, exe, args);
//                refreshGames();
//            }
//        }
//    }
//}
