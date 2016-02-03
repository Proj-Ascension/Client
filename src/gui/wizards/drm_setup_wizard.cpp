#include "drm_setup_wizard.h"
#include <libs/steam_vdf_parse.hpp>

#include <thread>
#include <future>

GameList addedVector;

/** DRMSetupWizard constructor
 * Defines the pages and initializes the database with the path given. Also sets up some window-related properties,
 * such as title and initial size.
 * \param parent Parent widget to draw from
 * \param dbPath Path to the database used
 */
DRMSetupWizard::DRMSetupWizard(QWidget* parent) : QWizard(parent)
{
    std::map<std::string, DRMType*> drmMap;
    SteamDRM* steam = new SteamDRM;
    OriginDRM* origin = new OriginDRM;
    UplayDRM* uplay = new UplayDRM;
    steam->checkExists();

#if defined(_WIN32) || defined(_WIN64) || defined(__APPLE__)
    origin->checkOriginExists();
#endif

#if defined(_WIN32) || defined (_WIN64)
    uplay->checkUplayExists();
#endif

    drmMap.insert(std::make_pair("Steam", steam));
    drmMap.insert(std::make_pair("Origin", origin));
    drmMap.insert(std::make_pair("Uplay", uplay));

    setPage(pages::INTRO, new IntroPage(this));
    setPage(pages::DRM, new DRMPage(drmMap, this));
    setPage(pages::RESULTS, new ResultsPage(drmMap, this));
    setPage(pages::FINAL, new FinalPage(this));
    setWindowTitle("Project Ascension setup");
    setFixedSize(QSize(700, 450));
    addedVector.erase(addedVector.begin(), addedVector.end());
 }

/** IntroPage constructor
 * Defines some initial properties for the introduction page.
 * \param parent Parent widget to draw from
 */
IntroPage::IntroPage(QWidget* parent) : QWizardPage(parent)
{
    setTitle("Finding games");
    setSubTitle("This wizard will try to find all your games.");
    QGridLayout* layout = new QGridLayout(this);
    QLabel* label = new QLabel("Click next to attempt to find Steam, Origin and Uplay games where applicable.");
    layout->addWidget(label);
    setLayout(layout);
}

/** DRMPage constructor
 * Defines some initial properties for the DRM page.
 * \param parent Parent widget to draw from
 */
DRMPage::DRMPage(std::map<std::string, DRMType*> drmMap, QWidget *parent) : QWizardPage(parent), drmMap(drmMap)
{
    setTitle("Found installed candidates");
    layout = new QGridLayout(this);

    static int count = 0;
    for (auto& drm : drmMap)
    {
        layout->addWidget(drm.second->getPlatformLabel(), count, 0, 0);
        layout->addWidget(drm.second->getDescLabel(), count+1, 0, 0);
        layout->addWidget(drm.second->getStatusLabel(), count, 1, 0);
        count += 3;
    }

    setLayout(layout);
}

/** ResultsPage constructor
 * Defines some initial properties for the results page.
 */
ResultsPage::ResultsPage(std::map<std::string, DRMType*> drmMap, QWidget* parent) : QWizardPage(parent)
{
    setSubTitle("We found the following on your system.");
    steam = static_cast<SteamDRM*>(drmMap.find(std::string("Steam"))->second);
    origin = static_cast<OriginDRM*>(drmMap.find(std::string("Origin"))->second);
    uplay = static_cast<UplayDRM*>(drmMap.find(std::string("Uplay"))->second);
}

/** FinalPage constructor
 * Defines some initial properties for the final page.
 */
FinalPage::FinalPage(QWidget* parent) : QWizardPage(parent)
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

    bool steamFound = steam->getIsInstalled();
    bool originFound = origin->getIsInstalled();
    bool uplayFound = uplay->getIsInstalled();

    if (!uplayFound && !steamFound && !originFound)
    {
        setTitle(title() + "no games.");
        setSubTitle("Install Steam, Origin and/or Uplay to find games with this wizard, or check current installation(s).");
    }
    else
    {
        setSubTitle("Change the title for each game by clicking the text box and editing.");
        tabWidget = new QTabWidget(this);
        topLayout = new QGridLayout(this);
        QProgressDialog* dialog = new QProgressDialog(this);
        dialog->setCancelButtonText("Cancel");
        dialog->setRange(0, 3);
        dialog->setWindowTitle("Project Ascension");
        dialog->setLabelText("Working");
        dialog->setWindowModality(Qt::NonModal);
        dialog->show();
        dialog->setValue(0);
        QApplication::processEvents();

        if (steamFound)
        {
			steam->findGames();
            GameList steamVector = steam->getGames();
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
            tabWidget->addTab(steam->createPane(this), "Steam");
        }
        dialog->setValue(1);
        QApplication::processEvents();

        if (originFound)
        {
			origin->findGames();
            pt::ptree originTree = origin->getGames();
            int count = originTree.get<int>("games.count");
            if (uplayFound)
            {
                setTitle(title() + QString::number(count) + QString(" Origin game") + (count == 1 ? QString(" and "):QString("s and ")));
            }
            else
            {
                setTitle(title() + QString::number(count) + QString(" Origin game") + (count == 1 ? QString("."):QString("s.")));
            }

            tabWidget->addTab(origin->createPane(this), "Origin");
        }
        dialog->setValue(2);
        QApplication::processEvents();

        if (uplayFound)
        {
			uplay->findGames();
            pt::ptree uplayTree = uplay->getGames();
            int count = uplayTree.get<int>("games.count");
            setTitle(title() + QString::number(count) + QString(" Uplay game") + (count == 1 ? QString(".") : QString("s.")));

            tabWidget->addTab(uplay->createPane(this), "Uplay");
        }
        dialog->setValue(3);
        QApplication::processEvents();

        dialog->close();


        selectAllBtn = new QPushButton("Select all");
        deselectAllBtn = new QPushButton("Deselect all");
        invertBtn = new QPushButton("Invert selection");
        connect(selectAllBtn, SIGNAL(clicked()), this, SLOT(selectAll()));
        connect(deselectAllBtn, SIGNAL(clicked()), this, SLOT(deselectAll()));
        connect(invertBtn, SIGNAL(clicked()), this, SLOT(invert()));

        topLayout->addWidget(tabWidget);
        QHBoxLayout* boxLayout = new QHBoxLayout();
        boxLayout->addWidget(selectAllBtn);
        boxLayout->addWidget(deselectAllBtn);
        boxLayout->addWidget(invertBtn);
        topLayout->addLayout(boxLayout, 1, 0, 0);
        connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabSelected()));
        setLayout(topLayout);
    }
}

/** Overloads the nextId function to add the selected games from the main results widget to a vector called "addedVector",
 * which will be added to the main database at a later point.
 * \see QWizardPage::nextId()
 */
int ResultsPage::nextId() const
{
    if (steam->getIsInstalled())
    {
        int steamOffset = 0;
        for (auto i : steam->getButtonGroup()->buttons())
        {
            if (i->isChecked())
            {
                std::cout << "Adding " << i->text().toLocal8Bit().constData() << std::endl;
                addedVector.push_back(steam->getGames().at(steamOffset));
            }
            steamOffset++;
        }
    }
    if (origin->getIsInstalled())
    {
        for (auto i : origin->getButtonGroupVector())
        {
            for (auto group : i->buttons())
            {
                if (group->isChecked())
                {
                    QDir path = group->text().remove("Executable: ");
                    std::string name = QString(QDir::cleanPath(
                        path.filePath("").remove(origin->getRootDir().filePath((""))))).toLocal8Bit().constData();
                    std::vector<std::string> strSplit;
                    boost::split(strSplit, name, boost::is_any_of("/"));
                    name = strSplit.at(1);

                    std::cout << "Adding " << name << std::endl;
                    unsigned int count = Database::getInstance().getGameCount();
                    addedVector.push_back({count, QString::fromStdString(name),
                                           origin->getRootDir().filePath(QString::fromStdString(name)),
                                           path.filePath(""), ""});
                }
            }
        }
    }
    if (uplay->getIsInstalled())
    {
        for (auto i : uplay->getButtonGroupVector())
        {

            for (auto group : i->buttons())
            {
                if (group->isChecked())
                {
                    QDir path = group->text().remove("Executable: ");
                    std::string name = QString(QDir::cleanPath(path.filePath("").remove(uplay->getRootDir().filePath((""))))).toLocal8Bit().constData();
                    std::vector<std::string> strSplit;
                    boost::split(strSplit, name, boost::is_any_of("/"));
                    name = strSplit.at(1);

                    std::cout << "Adding " << name << std::endl;
                    unsigned int count = Database::getInstance().getGameCount();
                    addedVector.push_back({count, QString::fromStdString(name), uplay->getRootDir().filePath(QString::fromStdString(name)), path.filePath(""), ""});
                }
            }
        }
    }
    return pages::FINAL;
}

DRMType* ResultsPage::getCurrentDRM()
{
    switch(tabWidget->currentIndex())
    {
        case 0:
            return steam;
        case 1:
            return origin;
        case 2:
            return uplay;
        default:
            return nullptr;
    }
}

/** Slot to set all buttons to checked */
void ResultsPage::selectAll()
{
    for (auto i : getCurrentDRM()->getButtonGroup()->buttons())
    {
        i->setChecked(true);
    }
}

/** Slot to set all buttons to unchecked */
void ResultsPage::deselectAll()
{
    for (auto i : getCurrentDRM()->getButtonGroup()->buttons())
    {
        i->setChecked(false);
    }
}

/** Slot to set all buttons to the opposite state */
void ResultsPage::invert()
{
    for (auto i : getCurrentDRM()->getButtonGroup()->buttons())
    {
        i->setChecked(!i->isChecked());
    }
}

/** Slot to update the title based upon the tab. Steam (yet) doesn't generate multiple executables, so the 2nd part
 * isn't needed.
 */
void ResultsPage::tabSelected()
{
    if (tabWidget->currentIndex() == 0)
    {
        setSubTitle("Change the title for each game by clicking the text box and editing.");
        selectAllBtn->show();
        deselectAllBtn->show();
        invertBtn->show();
    }
    else
    {
        setSubTitle("Change the title for each game by clicking the text box and editing. Choose which executable to use from the tree view.");
        selectAllBtn->hide();
        deselectAllBtn->hide();
        invertBtn->hide();
    }
}

/** Initializes the final page. This function is called when the NextButton on the previous page is clicked,
 * which allows the state of the previous page to be transferred to this one.
 * The function then loops through the addedVector and adds the games to the database.
*/
void FinalPage::initializePage()
{
    std::sort(addedVector.begin(), addedVector.end(), [&](const Game& g1, const Game& g2){return g1.gameName < g2.gameName;});
    Database::getInstance().addGames(addedVector);
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