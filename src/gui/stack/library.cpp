#include "library.h"
#include "../wizards/add_game_wizard.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>
#include <QFileSystemWatcher>

/** Library constructor
 * Initialize the library UI and generate an initial list of all the games available.
 * \param p Inherited palette configuration for setting StyleSheets.
 * \param parent Pointer to parent widget.
*/
Library::Library(QSettings* p, QWidget* parent)
    : QWidget(parent),
      runningProcess(new QProcess(this))
{
    this->setObjectName("libraryUI");

    this->setStyleSheet("QPushButton {"
                                "color: " + p->value("Primary/LightText").toString() + "; "
                                "background-color: " + p->value("Primary/DarkElement").toString() + "; "
                                "border: none; margin: 0px; padding: 0px;} "
                                "QPushButton:hover {"
                                "background-color: " + p->value("Primary/InactiveSelection").toString() + ";} ");

    QFont buttonFont("SourceSansPro", 12);

    // Main Library Layout
    QHBoxLayout* mainHLayout = new QHBoxLayout(this);
    mainHLayout->setMargin(0);

    // Library Sidebar Widget
    QWidget* sidebar = new QWidget(this);
    sidebar->setObjectName("librarySidebar");
    sidebar->setMinimumSize(210, 0);
    sidebar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    sidebar->setStyleSheet("#librarySidebar { background-color: " + p->value("Primary/SecondaryBase").toString() + ";}");
    mainHLayout->addWidget(sidebar);

    // Sidebar Vertical Layout
    QVBoxLayout* sidebarVLayout = new QVBoxLayout(sidebar);

    // Sidebar List Widget
    sidebarGameList = new QListWidget(sidebar);
    sidebarGameList->setObjectName("sidebarGameList");
    sidebarGameList->setStyleSheet("background-color: " + p->value("Primary/TertiaryBase").toString() + "; "
                                   "color: " + p->value("Primary/LightText").toString() + ";");
    sidebarGameList->setFont(QFont("SourceSansPro", 9));
    sidebarVLayout->addWidget(sidebarGameList);

    // Sidebar Horizontal Button Layout
    QHBoxLayout* sidebarButtonHLayout = new QHBoxLayout();
    sidebarVLayout->addLayout(sidebarButtonHLayout);

    // Add Game
    QPushButton* addGame = new QPushButton("Add Game", sidebar);
    addGame->setFont(buttonFont);
    sidebarButtonHLayout->addWidget(addGame);

    // Remove Game
    QPushButton* removeGame = new QPushButton("Remove Game", sidebar);
    removeGame->setFont(buttonFont);
    sidebarButtonHLayout->addWidget(removeGame);

    // Main Content Widget
    QWidget* content = new QWidget(this);
    content->setObjectName("libraryContent");
    content->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    content->setStyleSheet("#libraryContent {background-color: " + p->value("Primary/SecondaryBase").toString() + ";}");
    mainHLayout->addWidget(content);

    // Main Content Grid Layout
    QGridLayout* contentGrid = new QGridLayout(content);

    // Launch Game Button
    QPushButton* launchGame = new QPushButton("Launch Game", content);
    launchGame->setFont(buttonFont);
    contentGrid->addWidget(launchGame, 0, 0);

    if (!Database::getInstance().init())
    {
        QMessageBox error;
        error.critical(0, "Error!", "An error occured while trying to load the database.");
        exit(EXIT_FAILURE);
    }

    connect(addGame, SIGNAL(clicked()), this, SLOT(onAddGameClicked()));
    connect(removeGame, SIGNAL(clicked()), this, SLOT(onRemoveGameClicked()));
    connect(launchGame, SIGNAL(clicked()), this, SLOT(onLaunchGameClicked()));

    connect(runningProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finished(int, QProcess::ExitStatus)));
    connect(runningProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(onLaunchError(QProcess::ProcessError)));

    QList<Game> games = Database::getInstance().getGames();
    for (auto game : games)
    {
        qDebug() << game.id << game.gameName << game.gameDirectory << game.executablePath;
    }

    QFileSystemWatcher* watcher = new QFileSystemWatcher;
    watcher->addPath(QDir(".").filePath("ascension.db"));
    connect(watcher, SIGNAL(fileChanged(QString)), this, SLOT(refreshGames()));

    refreshGames();
}


Library::~Library()
{
    delete runningProcess;
}

/** Event handler for adding a game.
 * Prompts the user for various paths, and adds the final game to the database.
*/
void Library::onAddGameClicked()
{
    AddGameWizard* wiz = new AddGameWizard();
    wiz->show();
}

/** Event handler for removing a game.
*/
void Library::onRemoveGameClicked()
{
    auto selection = sidebarGameList->currentItem();
    if (selection != nullptr)
    {
        Database::getInstance().removeGameByName(selection->text());
        refreshGames();
    }
}

/** Event handler for launching a game.
 * Populates a message box on failure, or runs the games upon success.
*/
void Library::onLaunchGameClicked()
{
    if (!isProcessRunning())
    {
        auto selection = sidebarGameList->currentItem();
        if (selection != nullptr)
        {
            Game game = Database::getInstance().getGameByName(selection->text());
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

/** Launch a new QProcess using the passed exe and working directory.
 * \param file Location of the exe to run.
 * \param workingDirectory The directory that QProcess should spawn in.
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
 * \param file Location of the exe to run.
 * \param workingDirectory The directory that QProcess should spawn in.
 * \param args String of arguments to launch the executable with.
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
* Sort the list alphabetically by name
*/
void Library::refreshGames()
{
    sidebarGameList->clear();
    QList<Game> gameList = Database::getInstance().getGames();
    std::sort(gameList.begin(), gameList.end(), [&](const Game& g1, const Game& g2){return g1.gameName < g2.gameName; });
    for (auto game : gameList)
    {
        sidebarGameList->addItem(game.gameName);
    }
}

/** Attempt to handle process ending unexpectedly or fork.
 * \param exitCode Exit code to check.
 * \param exitStatus Status to check.
*/
void Library::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitCode != 0)
    {
        QMessageBox(QMessageBox::Warning, "Warning", "The game finished, but it claims to have encountered an error").exec();
    }
}

/** Handle errors before the process has launched.
 * \param error The error to translate.
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

/** Check if a process is running already.
 * \return Success/failure upon completion.
*/
bool Library::isProcessRunning() const
{
    // We shall consider "Starting" to be running here too
    return runningProcess->state() != QProcess::NotRunning;
}
