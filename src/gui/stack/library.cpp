#include "library.h"
#include "ui_library.h"
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
      ui(new Ui::Library),
      runningProcess(new QProcess(this))
{
    ui->setupUi(this);
    this->setObjectName("libraryUI");
    this->setStyleSheet("#leftSidebar {"
                        "background-color: " + p->value("Primary/SecondaryBase").toString() + ";} "
                        "#content {"
                        "background-color: " + p->value("Primary/SecondaryBase").toString() + ";} "
                        "QPushButton {"
                        "color: " + p->value("Primary/LightText").toString() + "; "
                        "background-color: " + p->value("Primary/DarkElement").toString() + "; "
                        "border: none; margin: 0px; padding: 0px;} "
                        "QPushButton:hover {"
                        "background-color: " + p->value("Primary/InactiveSelection").toString() + ";} "
                        "QListWidget {"
                        "background-color: " + p->value("Primary/TertiaryBase").toString() + "; "
                        "color: " + p->value("Primary/LightText").toString() + ";}");
    QFont buttonFont("SourceSansPro", 12);
    ui->addGame->setFont(buttonFont);
    ui->addGame->setText("Add Game");
    ui->removeGame->setFont(buttonFont);
    ui->removeGame->setText("Remove Game");
    ui->testLaunch->setFont(buttonFont);
    ui->testLaunch->setText("Launch Game");

    if (!db.init())
    {
        QMessageBox error;
        error.critical(0, "Error!", "An error occured while trying to load the database.");
        exit(EXIT_FAILURE);
    }

    connect(runningProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finished(int, QProcess::ExitStatus)));
    connect(runningProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(onLaunchError(QProcess::ProcessError)));

    QList<Game> games = db.getGames();
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
    AddGameWizard* wiz = new AddGameWizard();
    wiz->show();
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
    ui->gameListWidget->clear();
    QList<Game> gameList = db.getGames();
	std::sort(gameList.begin(), gameList.end(), [&](const Game& g1, const Game& g2){return g1.gameName < g2.gameName; });
    for (auto game : gameList)
    {
        ui->gameListWidget->addItem(game.gameName);
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
