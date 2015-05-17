#include "Library.h"
#include "ui_Library.h"

#include <QFileDialog>
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

void Library::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    processRunning = false;
}
