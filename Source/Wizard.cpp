#include <QtWidgets>
#include <QProcess>
#include <iostream>
#include "Wizard.h"
#include "Libs/SteamVdfParse.hpp"
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace pt = boost::property_tree;

Wizard::Wizard(QWidget* parent, QString dbPath) : QWizard(parent), db(dbPath + "ascension.db")
{
    setPage(pages::INTRO, new IntroPage());
    setPage(pages::DRM, new DRMPage(db));
    setPage(pages::FINAL, new FinalPage());
    setWindowTitle("Project Ascension setup");
}

IntroPage::IntroPage(QWidget* parent) : QWizardPage(parent)
{
    setTitle("Finding games");
    setSubTitle("This wizard will try to find all your games.");
    auto layout = new QGridLayout();
    auto label = new QLabel("Click next to attempt to find Steam, Origin and Uplay games where applicable.");
    layout->addWidget(label, 0, 0, 0, 0);
    setLayout(layout);
}

FinalPage::FinalPage(QWidget* parent) : QWizardPage(parent)
{
    setTitle("Done");
}

DRMPage::DRMPage(Database db, QWidget* parent) : QWizardPage(parent), db(db)
{
    setTitle("Checking for Steam, Origin and Uplay");
    auto layout = new QGridLayout();
    auto platformLabel = new QLabel("Steam");
    auto descLabel = new QLabel();
    auto statusLabel = new QLabel();
#if defined(__linux__)
    QProcess which;
    which.setProcessChannelMode(QProcess::MergedChannels);
    QStringList args;
    args << "steam";
    which.start("which", args);
    which.waitForFinished();
    if (which.exitCode() == 0)
    {
        statusLabel->setPixmap(QPixmap(":SystemMenu/Icons/Tick.svg"));
        descLabel->setText("Steam binary found, assuming $HOME/.local/share/Steam as steampath");
    }
    else
    {
        statusLabel->setPixmap(QPixmap(":SystemMenu/Icons/Cross.svg"));
        descLabel->setText("Steam binary not found. Install Steam or add to $PATH if already installed.");
    }
#endif
    layout->addWidget(platformLabel, 0, 0, 0);
    layout->addWidget(descLabel, 1, 0, 0);
    layout->addWidget(statusLabel, 0, 1, 0);
#if defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
    platformLabel = new QLabel("Origin");
    statusLabel = new QLabel();
    statusLabel->setPixmap(QPixmap("/home/elken/ClionProjects/Wizard/res/failed.svg"));
    layout->addWidget(platformLabel, 2, 0, 0);
    layout->addWidget(statusLabel, 2, 1, 0);
#elif defined(_WIN32) || defined (_WIN64)
    platformLabel = new QLabel("Uplay");
    statusLabel = new QLabel();
    statusLabel->setPixmap(QPixmap("/home/elken/ClionProjects/Wizard/res/failed.svg"));
    layout->addWidget(platformLabel, 4, 0, 0);
    layout->addWidget(statusLabel, 4, 1, 0);
#endif
    setLayout(layout);
}

ResultsPage::ResultsPage(QWidget* parent) : QWizardPage(parent)
{

}

QStringList DRMPage::findSteamGames(QDir steamRoot)
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
    bool directoryPlural = (steamDirectoryList.size() > 1);
    int ret = QMessageBox(QMessageBox::Question, "Found " + QString::number(steamDirectoryList.size()) + " director" + (directoryPlural ? "ies" : "y"), QString::number(steamDirectoryList.size()) + " directories have been found.\n\n" + pathString + "Proceed?", QMessageBox::Yes | QMessageBox::No).exec();
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

QStringList DRMPage::parseAcf(QDir steamRoot)
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
                    id = std::stoi(fileTree.get<std::string>("AppState.appID"));
                }
                catch (std::exception& e)
                {
                    id = std::stoi(fileTree.get<std::string>("AppState.appid"));
                }

                try
                {
                    auto game = games.at(id);
                    auto launch = game.pt.get_child("config.launch");

                    // Loop through the 0, 1, and 2 configurations
                    for (auto pair : launch)
                    {
                        pt::ptree section = pair.second;

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
                            path = QDir(path).filePath(QString::fromStdString(section.get("workingdir", "")));
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

                db.addGame(name, path, exe, args);
//                refreshGames();
            }
        }
    }
}