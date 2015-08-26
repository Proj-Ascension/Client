#include <QtWidgets>
#include <iostream>
#include "Wizard.h"
#include "Libs/SteamVdfParse.hpp"
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace pt = boost::property_tree;

Wizard::Wizard(QWidget* parent, QString dbPath) : QWizard(parent), db(dbPath + "ascension.db")
{
    setPage(pages::INTRO, new IntroPage());
    setPage(pages::DRM, new DRMPage());
    setPage(pages::RESULTS, new ResultsPage(db));
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
    setTitle("Checking for Steam, Origin and Uplay");
    auto steamBox = new QCheckBox();
    auto originBox = new QCheckBox();
    auto uplayBox = new QCheckBox();
    registerField("steamFound", steamBox);
    registerField("uplayFound", uplayBox);
    registerField("originFound", originBox);
    auto layout = new QGridLayout();
    auto platformLabel = new QLabel("<b>Steam</b>");
    auto descLabel = new QLabel();
    auto statusLabel = new QLabel();
    platformLabel->setTextFormat(Qt::TextFormat::RichText);
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
        steamBox->setChecked(true);
    }
    else
    {
        statusLabel->setPixmap(QPixmap(":SystemMenu/Icons/Cross.svg"));
        descLabel->setText("Steam binary not found. Install Steam or add to $PATH if already installed.");
    }
#elif defined(_WIN32) || defined(_WIN64)
    QSettings settings("HKEY_CURRENT_USER\\Software\\Valve\\Steam", QSettings::NativeFormat);
    if (!settings.value("SteamPath").isNull())
    {
        statusLabel->setPixmap(QPixmap(":/SystemMenu/Icons/Tick.svg"));
        descLabel->setText("Steam root found in " + settings.value("SteamPath").toString());
        steamBox->setChecked(true);
    }
    else
    {
        statusLabel->setPixmap(QPixmap(":/SystemMenu/Icons/Cross.svg"));
        descLabel->setText("Steam not found, verify installation and try again.");
        steamBox->setChecked(false);
    }
#endif
    layout->addWidget(platformLabel, 0, 0, 0);
    layout->addWidget(descLabel, 1, 0, 0);
    layout->addWidget(statusLabel, 0, 1, 0);
#if defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
    platformLabel = new QLabel("<b>Origin</b>");
    statusLabel = new QLabel();
    statusLabel->setPixmap(QPixmap(":/SystemMenu/Icons/Cross.svg"));
    layout->addWidget(platformLabel, 2, 0, 0);
    layout->addWidget(statusLabel, 2, 1, 0);
#endif

#if defined(_WIN32) || defined (_WIN64)
    platformLabel = new QLabel("<b>Uplay</b>");
    statusLabel = new QLabel();
    statusLabel->setPixmap(QPixmap(":/SystemMenu/Icons/Cross.svg"));
    layout->addWidget(platformLabel, 4, 0, 0);
    layout->addWidget(statusLabel, 4, 1, 0);
#endif

    setLayout(layout);
}

ResultsPage::ResultsPage(Database db, QWidget* parent) : QWizardPage(parent), db(db)
{
    setSubTitle("We found the following on your system.");
}

void ResultsPage::initializePage()
{
    qDebug() << field("steamFound").toBool();
    QDir steamRoot;
#if defined(__linux__)
    const QDir steamRoot = QDir(QDir::homePath() + "/.local/share/Steam/");
#elif defined(_WIN32) || defined(_WIN64)
    QSettings settings("HKEY_CURRENT_USER\\Software\\Valve\\Steam", QSettings::NativeFormat);
    if (!settings.value("SteamPath").isNull())
    {
        steamRoot = settings.value("SteamPath").toString();
    }
#endif
    findSteamGames(steamRoot);
    setTitle(QString("We found ") + QString::number(steamVector.size()) + QString(" game") + (steamVector.size() >= 2 ? QString("s."):QString(".")));
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

void ResultsPage::findSteamGames(QDir steamRoot)
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