#include "Library.h"
#include "ui_Library.h"


#include <QFileDialog>
#include <QInputDialog>
#include <QProcess>
#include <QMessageBox>
#include <QDebug>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>

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
    findSteamGames();
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
            file = dir.absoluteFilePath(fileList.at(2));// USUALLY this is the executable (after ., ..)
        #endif

        QFileDialog wdDialog; // Working Directory
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

void Library::findSteamGames()
{
    #if defined(_WIN32) || defined(_WIN64)
        QSettings settings("HKEY_CURRENT_USER\\Software\\Valve\\Steam", QSettings::NativeFormat);
        QDir steamRoot(settings.value("SteamPath").toString());
    #elif defined(__apple__)
        // TODO: however OS X handles steam
    #elif defined(__linux__)
        QDir steamRoot(QDir::home().filePath(".steam/steam"));
    #else
        QMessageBox(QMessageBox::Critical, "Error", "Platform doesn't support steam.");
    #endif

    if (steamRoot.exists())
    {
        pt::ptree libraryFolders;
        pt::read_info(steamRoot.filePath("steamapps/libraryfolders.vdf").toLocal8Bit().constData(), libraryFolders);
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
        int ret = QMessageBox(QMessageBox::Question, "Found " + QString::number(steamDirectoryList.size()) + " director" + (directoryPlural ? "ies" : "y"), QString::number(steamDirectoryList.size()) + " directories have been found.\n\n" + pathString + "Proceed?", QMessageBox::Yes | QMessageBox::No).exec();
        switch(ret)
        {
            case QMessageBox::Yes:
                parseAcf();
                break;
            case QMessageBox::No:
                break;
            default:
                break;
        }
    }
}

void Library::parseAcf()
{
    // TODO: This stuff needs its own thread
    for(QString iter : steamDirectoryList)
    {
        QDir steamAppsDir(iter);
        steamAppsDir = steamAppsDir.filePath("steamapps");
        QStringList fileList = steamAppsDir.entryList(QStringList("*.acf"), QDir::Files | QDir::NoSymLinks);

        for (auto fileIter : fileList)
        {
            pt::ptree fileTree;
            std::string acfDir = steamAppsDir.filePath(fileIter).toLocal8Bit().constData();
            pt::read_info(acfDir, fileTree);

            QString name = QString::fromStdString(fileTree.get<std::string>("AppState.name"));
            // TODO: Either add SteamID to db, or add getGameByPath
            if (!std::get<0>(db.isExistant(name)))
            {
                QString path = steamAppsDir.filePath("common/" + QString::fromStdString(fileTree.get<std::string>("AppState.installdir")));
                QString exe;
                QStringList exeList = QDir(path).entryList(QDir::Files | QDir::NoSymLinks | QDir::Executable);

                QFileDialog exeDialog;
                exeDialog.setWindowTitle("Select Executable");
                exeDialog.setFileMode(QFileDialog::ExistingFile);
                exeDialog.setDirectory(path);
                if (exeDialog.exec())
                {
                    exe = exeDialog.selectedFiles().at(0);
                }

                db.addGame(name, path, exe);
                refreshGames();
            }
        }
    }
}
