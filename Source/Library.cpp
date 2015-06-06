#include "Library.h"
#include "ui_Library.h"

#include <QFileDialog>
#include <QInputDialog>
#include <QProcess>
#include <QMessageBox>
#include <QDebug>

Library::Library(Database db) :
    QWidget(0),
    db(db),
    ui(new Ui::Library)
{
    ui->setupUi(this);
    this->setObjectName("libraryUI");
    runningProcess = new QProcess(this);
    connect(runningProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finished(int, QProcess::ExitStatus)));

    QList<Game> games = db.getGames();
    for (auto game : games)
    {
        qDebug() << game.id << game.gameName << game.gameDirectory << game.executablePath;
    }

    refreshGames();
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
        Game game = db.getGameByName(ui->gameListWidget->item(ui->gameListWidget->currentRow())->text());
        runProcess(game.executablePath, game.gameDirectory);
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

    if (exeDialog.exec())
    {
        QStringList files = exeDialog.selectedFiles();
        QString exe = files.at(0);
        #ifdef Q_WS_MACX
            //Get the binary from the app bundle
            QDir dir(file + "/Contents/MacOS");
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
    db.removeGameByName(ui->gameListWidget->item(ui->gameListWidget->currentRow())->text());
    refreshGames();
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
        runningProcess->start("\"" + file + "\"");
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
}

bool Library::isProcessRunning() const
{
    // We shall consider "Starting" to be running here too
    return runningProcess->state() != QProcess::NotRunning;
}
