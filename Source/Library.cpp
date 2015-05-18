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
    processRunning = false;
    connect(runningProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finished(int, QProcess::ExitStatus)));

    QList<Game> games = db.getGames();
    for (auto game : games)
    {
        qDebug() << game.id << ", " << game.gameName << ", " << game.gameDirectory << ", " << game.executablePath;
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
    if (!processRunning)
    {
        QFileDialog dialog;
        dialog.setWindowTitle("Select Executable");
        dialog.setFileMode(QFileDialog::ExistingFile);

        if (dialog.exec())
        {
            QStringList files = dialog.selectedFiles();
            QString file = files.at(0);
            #ifdef Q_WS_MACX
                //Get the binary from the app bundle
                QDir dir(file + "/Contents/MacOS");
                QStringList fileList = dir.entryList();
                file = dir.absoluteFilePath(fileList.at(2));//USUALLY this is the executable (after ., ..)
            #endif
            runProcess(file);
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

void Library::runProcess(QString location)
{
    // TODO: Implement some threading
    if (!processRunning)
    {
        runningProcess->start(location);
        runningProcess->waitForStarted();
        processRunning = true;
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
    processRunning = false;
}
