#include "drm_setup_wizard.h"
#include "../../libs/steam_vdf_parse.hpp"
#include "../../drm/steam_drm.h"
#include "../../drm/origin_drm.h"
#include "../../drm/uplay_drm.h"
#include "../../database.h"
#include "../stack/library.h"

#include <QtWidgets>


#include <iostream>
#include <thread>
#include <future>

GameList addedVector;
QList<QButtonGroup*> originBtnGroupVector;
QList<QButtonGroup*> uplayBtnGroupVector;

/** DRMSetupWizard constructor
 * Defines the pages and initializes the database with the path given. Also sets up some window-related properties,
 * such as title and initial size.
 * \param parent Parent widget to draw from
 * \param dbPath Path to the database used
 */
DRMSetupWizard::DRMSetupWizard(QWidget* parent, QString dbPath) : QWizard(parent), db(dbPath + "ascension.db")
{
    std::map<std::string, DRMType*> drmMap;
    SteamDRM* steam = new SteamDRM;
    steam->checkExists();
    if (steam->getIsInstalled())
    {
        drmMap.insert(std::make_pair(std::string("Steam"), steam));
    }

#if defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
    OriginDRM* origin = new OriginDRM;
    origin->checkOriginExists();
    if (origin->getIsInstalled())
    {
        drmMap.insert(std::make_pair("Origin", origin));
    }
#endif

#if defined(_WIN32) || defined (_WIN64)
    UplayDRM* uplay = new UplayDRM;
    uplay->checkUplayExists();
    if (uplay->getIsInstalled())
    {
        drmMap.insert(std::make_pair("Uplay", uplay));
    }
#endif

    setPage(pages::INTRO, new IntroPage());
    setPage(pages::DRM, new DRMPage(drmMap));
    setPage(pages::RESULTS, new ResultsPage(drmMap, db));
    setPage(pages::FINAL, new FinalPage(db));
    setWindowTitle("Project Ascension setup");
    setFixedSize(QSize(700, 450));
    addedVector.erase(addedVector.begin(), addedVector.end());
    if (!db.init())
    {
        QMessageBox error;
        error.critical(0, "Error!", "An error occured while trying to load the database.");
        exit(EXIT_FAILURE);
    }
 }

/** IntroPage constructor
 * Defines some initial properties for the introduction page.
 * \param parent Parent widget to draw from
 */
IntroPage::IntroPage(QWidget* parent) : QWizardPage(parent)
{
    setTitle("Finding games");
    setSubTitle("This wizard will try to find all your games.");
    auto layout = new QGridLayout();
    auto label = new QLabel("Click next to attempt to find Steam, Origin and Uplay games where applicable.");
    layout->addWidget(label);
    setLayout(layout);
}

/** DRMPage constructor
 * Defines some initial properties for the DRM page.
 * \param parent Parent widget to draw from
 */
DRMPage::DRMPage(std::map<std::string, DRMType*> drmMap, QWidget *parent) : QWizardPage(parent)
{
    setTitle("Found installed candidates");
    layout = new QGridLayout();

    static int count = 0;
    for (auto& drm : drmMap)
    {
        layout->addWidget(drm.second->getPlatformLabel(), count, 0, 0);
        layout->addWidget(drm.second->getDescLabel(), count+1, 0, 0);
        layout->addWidget(drm.second->getStatusLabel(), count, 0, 0);
        count += 3;
    }

    setLayout(layout);
}

/** ResultsPage constructor
 * Defines some initial properties for the results page.
 */
ResultsPage::ResultsPage(std::map<std::string, DRMType*> drmMap, Database db, QWidget* parent) : QWizardPage(parent), db(db), drmMap(drmMap)
{
    setSubTitle("We found the following on your system.");
}

/** FinalPage constructor
 * Defines some initial properties for the final page.
 */
FinalPage::FinalPage(Database db, QWidget* parent) : QWizardPage(parent), db(db)
{
    setTitle("Done");
}

/** Initializes the results page. This function is called when the NextButton on the previous page is clicked,
 * which allows the state of the previous page to be transferred to this one.
 * The function then runs each respective check*Exists function, and populates the various sections with correct information.
 */
void ResultsPage::initializePage()
{
    setTitle(QString("We found "));
    SteamDRM* steam = static_cast<SteamDRM*>(drmMap.find(std::string("Steam"))->second);
    OriginDRM* origin = static_cast<OriginDRM*>(drmMap.find(std::string("Origin"))->second);
    UplayDRM* uplay = static_cast<UplayDRM*>(drmMap.find(std::string("Uplay"))->second);

//    bool steamFound = steam->getIsInstalled();
//    bool originFound = origin->getIsInstalled();
//    bool uplayFound = uplay->getIsInstalled();

    bool steamFound = drmMap.find(std::string("Steam"))->second->getIsInstalled();
    bool originFound = drmMap.find(std::string("Origin"))->second->getIsInstalled();
    bool uplayFound = drmMap.find(std::string("Uplay"))->second->getIsInstalled();

    qDebug() << steamFound << originFound << uplayFound;
    if (!uplayFound && !steamFound && !originFound)
    {
        setTitle(title() + "no games.");
        setSubTitle("Install Steam, Origin and/or Uplay to find games with this wizard, or check current installation(s).");
    }
    else
    {
        setSubTitle("Change the title for each game by clicking the text box and editing.");
        btnGroup = new QButtonGroup();
        btnGroup->setExclusive(false);
        tabWidget = new QTabWidget();
        top_layout = new QGridLayout();
        if (steamFound)
        {
            steamLayout = new QGridLayout();
            steamScrollArea = new QScrollArea();
            steamViewport = new QWidget();
            auto t = std::async(std::launch::async, &SteamDRM::findGames, steam);
            t.get();
            int row = 0;
            if (uplayFound && originFound)
            {
                setTitle(title() + QString::number(steamVector.size()) + QString(" Steam game") + (steamVector.size() == 1 ? QString(", "):QString("s, ")));
            }
            else if (uplayFound || originFound)
            {
                setTitle(title() + QString::number(steamVector.size()) + QString(" Steam game") + (steamVector.size() == 1 ? QString(" and "):QString("s and ")));
            }
            else
            {
                setTitle(title() + QString::number(steamVector.size()) + QString(" Steam game") + (steamVector.size() == 1 ? QString("."):QString("s.")));
            }

            for (auto i : steamVector)
            {
                QCheckBox* checkBox = new QCheckBox("Executable not found");
                QLineEdit* name = new QLineEdit(i.gameName);
                name->setFixedWidth(350);
                checkBox->setStyleSheet("QLabel { color: red; }");
                for (auto dir : steamDirectoryList)
                {
                    if (i.executablePath.contains(dir))
                    {
                        checkBox->setStyleSheet("QLabel { color: black; }");
                        checkBox->setText("Executable:  " + i.executablePath.remove(QDir(dir).filePath("SteamApps/common")).remove(0, 1));
                        break;
                    }
                }
                steamLayout->addWidget(name, row, 0, Qt::AlignVCenter | Qt::AlignLeft);
                row++;
                steamLayout->addWidget(checkBox, row, 0, Qt::AlignVCenter | Qt::AlignLeft);
                row++;
                btnGroup->addButton(checkBox);
            }
            steamViewport->setLayout(steamLayout);
            steamScrollArea->setWidget(steamViewport);
            tabWidget->addTab(steamScrollArea, "Steam");
        }

        if (originFound)
        {
            originScrollArea = new QScrollArea();
            originViewport = new QWidget();
            originLayout = new QGridLayout();
            auto t = std::async(&ResultsPage::findOriginGames, this);
            t.get();
            int row = 0;
            int count = originTree.get<int>("games.count");
            if (uplayFound)
            {
                setTitle(title() + QString::number(count) + QString(" Origin game") + (count == 1 ? QString(" and "):QString("s and ")));
            }
            else
            {
                setTitle(title() + QString::number(count) + QString(" Origin game") + (count == 1 ? QString("."):QString("s.")));
            }


            for (pt::ptree::value_type& games : originTree.get_child("games"))
            {
                boost::optional<std::string> exeTest = games.second.get_optional<std::string>("exes");
                if (exeTest)
                {
                    QButtonGroup* group = new QButtonGroup();
                    QLineEdit* name = new QLineEdit(QString::fromStdString(games.second.get<std::string>("name")));
                    name->setFixedWidth(350);
                    QCheckBox* checkBox = new QCheckBox();
                    originLayout->addWidget(name, row, 0, Qt::AlignVCenter | Qt::AlignLeft);
                    row++;
                    for (auto& exe : games.second.get_child("exes"))
                    {
                        checkBox = new QCheckBox("Executable: " + QString::fromStdString(exe.second.data()));
                        group->addButton(checkBox);
                        originLayout->addWidget(checkBox, row, 0, Qt::AlignVCenter | Qt::AlignLeft);
                        row++;
                    }
                    originBtnGroupVector.push_back(group);
                }
            }
            originViewport->setLayout(originLayout);
            originScrollArea->setWidget(originViewport);
            tabWidget->addTab(originScrollArea, "Origin");
        }

        if (uplayFound)
        {
            uplayLayout = new QGridLayout();
            uplayScrollArea = new QScrollArea();
            uplayViewport = new QWidget();
            auto t = std::async(&ResultsPage::findUplayGames, this);
            t.get();
            int row = 0;
            int count = uplayTree.get<int>("games.count");
            setTitle(title() + QString::number(count) + QString(" Uplay game") + (count == 1 ? QString("."):QString("s.")));

            for (pt::ptree::value_type& games : uplayTree.get_child("games"))
            {
                boost::optional<std::string> exeTest = games.second.get_optional<std::string>("exes");
                if (exeTest)
                {
                    QButtonGroup* group = new QButtonGroup();
                    QLineEdit* name = new QLineEdit(QString::fromStdString(games.second.get<std::string>("name")));
                    name->setFixedWidth(350);
                    QCheckBox* checkBox = new QCheckBox();
                    uplayLayout->addWidget(name, row, 0, Qt::AlignVCenter | Qt::AlignLeft);
                    row++;
                    for (auto& exe : games.second.get_child("exes"))
                    {
                        checkBox = new QCheckBox("Executable: " + QString::fromStdString(exe.second.data()));
                        group->addButton(checkBox);
                        uplayLayout->addWidget(checkBox, row, 0, Qt::AlignVCenter | Qt::AlignLeft);
                        row++;
                    }
                    uplayBtnGroupVector.push_back(group);
                }
            }
            uplayViewport->setLayout(uplayLayout);
            uplayScrollArea->setWidget(uplayViewport);
            tabWidget->addTab(uplayScrollArea, "Uplay");
        }
//
//    if (!field("uplayFound").toBool() && !field("steamFound").toBool() && !field("originFound").toBool())
//    {
//        setTitle(title() + "no games.");
//        setSubTitle("Install Steam, Origin and/or Uplay to find games with this wizard, or check current installation(s).");
//    }
//    else
//    {
//        setSubTitle("Change the title for each game by clicking the text box and editing.");
//        btnGroup = new QButtonGroup();
//        btnGroup->setExclusive(false);
//        tabWidget = new QTabWidget();
//        top_layout = new QGridLayout();
//        if (field("steamFound").toBool())
//        {
//            steamLayout = new QGridLayout();
//            steamScrollArea = new QScrollArea();
//            steamViewport = new QWidget();
//            auto t = std::async(&ResultsPage::findSteamGames, this);
//            t.get();
//            int row = 0;
//            if (field("uplayFound").toBool() && field("originFound").toBool())
//            {
//                setTitle(title() + QString::number(steamVector.size()) + QString(" Steam game") + (steamVector.size() == 1 ? QString(", "):QString("s, ")));
//            }
//            else if (field("uplayFound").toBool() || field("originFound").toBool())
//            {
//                setTitle(title() + QString::number(steamVector.size()) + QString(" Steam game") + (steamVector.size() == 1 ? QString(" and "):QString("s and ")));
//            }
//            else
//            {
//                setTitle(title() + QString::number(steamVector.size()) + QString(" Steam game") + (steamVector.size() == 1 ? QString("."):QString("s.")));
//            }
//
//            for (auto i : steamVector)
//            {
//                QCheckBox* checkBox = new QCheckBox("Executable not found");
//                QLineEdit* name = new QLineEdit(i.gameName);
//                name->setFixedWidth(350);
//                checkBox->setStyleSheet("QLabel { color: red; }");
//                for (auto dir : steamDirectoryList)
//                {
//                    if (i.executablePath.contains(dir))
//                    {
//                        checkBox->setStyleSheet("QLabel { color: black; }");
//                        checkBox->setText("Executable:  " + i.executablePath.remove(QDir(dir).filePath("SteamApps/common")).remove(0, 1));
//                        break;
//                    }
//                }
//                steamLayout->addWidget(name, row, 0, Qt::AlignVCenter | Qt::AlignLeft);
//                row++;
//                steamLayout->addWidget(checkBox, row, 0, Qt::AlignVCenter | Qt::AlignLeft);
//                row++;
//                btnGroup->addButton(checkBox);
//            }
//            steamViewport->setLayout(steamLayout);
//            steamScrollArea->setWidget(steamViewport);
//            tabWidget->addTab(steamScrollArea, "Steam");
//        }
//
//        if (field("originFound").toBool())
//        {
//            originScrollArea = new QScrollArea();
//            originViewport = new QWidget();
//            originLayout = new QGridLayout();
//            auto t = std::async(&ResultsPage::findOriginGames, this);
//            t.get();
//            int row = 0;
//            int count = originTree.get<int>("games.count");
//            if (field("uplayFound").toBool())
//            {
//                setTitle(title() + QString::number(count) + QString(" Origin game") + (count == 1 ? QString(" and "):QString("s and ")));
//            }
//            else
//            {
//                setTitle(title() + QString::number(count) + QString(" Origin game") + (count == 1 ? QString("."):QString("s.")));
//            }
//
//
//            for (pt::ptree::value_type& games : originTree.get_child("games"))
//            {
//                boost::optional<std::string> exeTest = games.second.get_optional<std::string>("exes");
//                if (exeTest)
//                {
//                    QButtonGroup* group = new QButtonGroup();
//                    QLineEdit* name = new QLineEdit(QString::fromStdString(games.second.get<std::string>("name")));
//                    name->setFixedWidth(350);
//                    QCheckBox* checkBox = new QCheckBox();
//                    originLayout->addWidget(name, row, 0, Qt::AlignVCenter | Qt::AlignLeft);
//                    row++;
//                    for (auto& exe : games.second.get_child("exes"))
//                    {
//                        checkBox = new QCheckBox("Executable: " + QString::fromStdString(exe.second.data()));
//                        group->addButton(checkBox);
//                        originLayout->addWidget(checkBox, row, 0, Qt::AlignVCenter | Qt::AlignLeft);
//                        row++;
//                    }
//                    originBtnGroupVector.push_back(group);
//                }
//            }
//            originViewport->setLayout(originLayout);
//            originScrollArea->setWidget(originViewport);
//            tabWidget->addTab(originScrollArea, "Origin");
//        }
//
//        if (field("uplayFound").toBool())
//        {
//            uplayLayout = new QGridLayout();
//            uplayScrollArea = new QScrollArea();
//            uplayViewport = new QWidget();
//            auto t = std::async(&ResultsPage::findUplayGames, this);
//            t.get();
//            int row = 0;
//            int count = uplayTree.get<int>("games.count");
//            setTitle(title() + QString::number(count) + QString(" Uplay game") + (count == 1 ? QString("."):QString("s.")));
//
//            for (pt::ptree::value_type& games : uplayTree.get_child("games"))
//            {
//                boost::optional<std::string> exeTest = games.second.get_optional<std::string>("exes");
//                if (exeTest)
//                {
//                    QButtonGroup* group = new QButtonGroup();
//                    QLineEdit* name = new QLineEdit(QString::fromStdString(games.second.get<std::string>("name")));
//                    name->setFixedWidth(350);
//                    QCheckBox* checkBox = new QCheckBox();
//                    uplayLayout->addWidget(name, row, 0, Qt::AlignVCenter | Qt::AlignLeft);
//                    row++;
//                    for (auto& exe : games.second.get_child("exes"))
//                    {
//                        checkBox = new QCheckBox("Executable: " + QString::fromStdString(exe.second.data()));
//                        group->addButton(checkBox);
//                        uplayLayout->addWidget(checkBox, row, 0, Qt::AlignVCenter | Qt::AlignLeft);
//                        row++;
//                    }
//                    uplayBtnGroupVector.push_back(group);
//                }
//            }
//            uplayViewport->setLayout(uplayLayout);
//            uplayScrollArea->setWidget(uplayViewport);
//            tabWidget->addTab(uplayScrollArea, "Uplay");
//        }
//
//        QPushButton* selectAllBtn = new QPushButton("Select all");
//        QPushButton* deselectAllBtn = new QPushButton("Deselect all");
//        QPushButton* invertBtn = new QPushButton("Invert selection");
//        connect(selectAllBtn, SIGNAL(clicked()), this, SLOT(selectAll()));
//        connect(deselectAllBtn, SIGNAL(clicked()), this, SLOT(deselectAll()));
//        connect(invertBtn, SIGNAL(clicked()), this, SLOT(invert()));
//
//        top_layout->addWidget(tabWidget);
//        QHBoxLayout* boxLayout = new QHBoxLayout();
//        boxLayout->addWidget(selectAllBtn);
//        boxLayout->addWidget(deselectAllBtn);
//        boxLayout->addWidget(invertBtn);
//        top_layout->addLayout(boxLayout, 1, 0, 0);
//        connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabSelected()));
//        setLayout(top_layout);
    }
}

/** Overloads the nextId function to add the selected games from the main results widget to a vector called "addedVector",
 * which will be added to the main database at a later point.
 * \see QWizardPage::nextId()
 */
int ResultsPage::nextId() const
{
    if (field("uplayFound").toBool() || field("steamFound").toBool() || field("originFound").toBool())
    {
        int steamOffset = 0;
        for (auto i : btnGroup->buttons())
        {
            if (i->isChecked())
            {
                std::cout << "Adding " << i->text().toLocal8Bit().constData() << std::endl;
                addedVector.push_back(steamVector.at(steamOffset));
            }
            steamOffset++;
        }
        for (auto i : originBtnGroupVector)
        {
            for (auto group : i->buttons())
            {
                if (group->isChecked())
                {
                    QDir path = group->text().remove("Executable: ");
                    std::string name = QString(QDir::cleanPath(path.filePath("").remove(originRoot.filePath((""))))).toLocal8Bit().constData();
                    std::vector<std::string> strSplit;
                    boost::split(strSplit, name, boost::is_any_of("/"));
                    name = strSplit.at(1);

                    std::cout << "Adding " << name << std::endl;
                    unsigned int count = db.getGameCount();
                    addedVector.push_back({count, QString::fromStdString(name), originRoot.filePath(QString::fromStdString(name)), path.filePath(""), ""});
                }
            }
        }
        for (auto i : uplayBtnGroupVector)
        {

            for (auto group : i->buttons())
            {
                if (group->isChecked())
                {
                    QDir path = group->text().remove("Executable: ");
                    std::string name = QString(QDir::cleanPath(path.filePath("").remove(uplayRoot.filePath((""))))).toLocal8Bit().constData();
                    std::vector<std::string> strSplit;
                    boost::split(strSplit, name, boost::is_any_of("/"));
                    name = strSplit.at(1);

                    std::cout << "Adding " << name << std::endl;
                    unsigned int count = db.getGameCount();
                    addedVector.push_back({count, QString::fromStdString(name), uplayRoot.filePath(QString::fromStdString(name)), path.filePath(""), ""});
                }
            }
        }
    }
    return pages::FINAL;
}

/** Slot to set all buttons to checked */
void ResultsPage::selectAll()
{
    for (auto i : btnGroup->buttons())
    {
        i->setChecked(true);
    }
}

/** Slot to set all buttons to unchecked */
void ResultsPage::deselectAll()
{
    for (auto i : btnGroup->buttons())
    {
        i->setChecked(false);
    }
}
/** Slot to set all buttons to the opposite state */
void ResultsPage::invert()
{
    for (auto i : btnGroup->buttons())
    {
        i->setChecked(!i->isChecked());
    }
}

/** Slot to update the title based upon the tab. Steam (yet) doesn't generate multiple executables, so the 2nd part
 * isn't needed.
 */
void ResultsPage::tabSelected()
{
    if (tabWidget->currentIndex() >= 1)
    {
        setSubTitle("Change the title for each game by clicking the text box and editing. Choose which executable to use from the tree view.");
    }
    else
    {
        setSubTitle("Change the title for each game by clicking the text box and editing.");
    }
}

/** Using steamRoot, which is initialized earlier in the wizard, find the location of every Steam library folder, and
 * run the parseAcf function on every *.acf file found within the directory. This file gives basic information about the game;
 * such as directory within steamapps, name, etc.
 * \see findOriginGames()
 * \see findUplayGames()
 */
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

    parseAcf(steamRoot);
}

/** Using originRoot, which is initialized earlier in the wizard, utilize `recursiveFindFiles()` to find every executable within
 * each respective directory. Some directories will contain more than one executable, so it's up to the user to select the correct one.
 * \see findSteamGames()
 * \see findUplayGames()
 */
void ResultsPage::findOriginGames()
{
    originRoot.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList folderList = originRoot.entryList();
    int count = 0;

    for (auto i : folderList)
    {
        if (i != "DownloadCache")
        {
            pt::ptree& node = originTree.add("games.game", "");
            QDir dir(originRoot.absoluteFilePath(i));
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
    originTree.add("games.count", count);
}

/** Using uplayRoot, which is initialized earlier in the wizard, utilize `recursiveFindFiles()` to find every executable within
 * each respective directory. Some directories will contain more than one executable, so it's up to the user to select the correct one.
 * \see findSteamGames()
 * \see findOriginGames()
 */
void ResultsPage::findUplayGames()
{
    uplayRoot.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    QStringList folderList = uplayRoot.entryList();
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
            QDir dir(uplayRoot.absoluteFilePath(i));
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

/** Debug function to print out a ptree */
void ResultsPage::printTree(pt::ptree &pt, int level)
{
    if (pt.empty())
    {
        std::cerr << "\""<< pt.data()<< "\"";
    }
    else
    {
        auto indent = [&](int level){ std::string s; for (int i=0; i<level; i++) s += "  "; return s; };
        if (level) std::cerr << std::endl;
        std::cerr << indent(level) << "{" << std::endl;
        for (pt::ptree::iterator pos = pt.begin(); pos != pt.end();)
        {
            std::cerr << indent(level+1) << "\"" << pos->first << "\": ";
            printTree(pos->second, level + 1);
            ++pos;
            if (pos != pt.end())
            {
                std::cerr << ",";
            }
            std::cerr << std::endl;
        }
        std::cerr << indent(level) << " }";
    }
}

/** For a given steamRoot directory, find every appmanifest file, parse it and then use the retrived id to parse the appinfo.vdf
 * file to ascertain extended information; such as executables included within the game. This currently only retrieves one exe, more
 * work will need to go into creating a fast and efficient algorithm to parse multiple games.
 * \param steamRoot The root directory to user's Steam installation.
 */
void ResultsPage::parseAcf(QDir steamRoot)
{

}

/** For a given dir, find every file contained within. It's up to the implementer of this function to insure that the
 * QDir has the relevant filters.
 * \param dir Directory to search in.
 */
QStringList ResultsPage::recursiveFindFiles(QDir dir)
{
    QStringList dirList;
    QDirIterator it(dir, QDirIterator::Subdirectories);

    while (it.hasNext())
    {
        QDir cur = it.next();
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

    return dirList;
}

/** Initializes the final page. This function is called when the NextButton on the previous page is clicked,
 * which allows the state of the previous page to be transferred to this one.
 * The function then loops through the addedVector and adds the games to the database.
*/
void FinalPage::initializePage()
{
    std::sort(addedVector.begin(), addedVector.end(), [&](const Game& g1, const Game& g2){return g1.gameName < g2.gameName;});
    db.addGames(addedVector);
    setSubTitle(QString("Added ") + QString::number(addedVector.size()) + " games to the database. Click finish to complete the wizard.");
}

/** Overloads the nextId function */
int IntroPage::nextId() const
{
    return pages::DRM;
}

/** Overloads the nextId function */
int DRMPage::nextId() const
{
    return pages::RESULTS;
}
