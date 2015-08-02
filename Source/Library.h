#pragma once

#include "Database.h"

#include <QWidget>
#include <QProcess>

/*! Library UI namespace. */
namespace Ui
{
    class Library;
}

/*! Library class
 * Class to handle the library section of the launcher 
*/
class Library : public QWidget
{
    Q_OBJECT

public:
    Library(Database db);
    ~Library();

private slots:
    void on_testLaunch_clicked();
    void on_addGame_clicked();
    void finished(int exitCode, QProcess::ExitStatus exitStatus);
    void onLaunchError(QProcess::ProcessError error);

    void on_removeGame_clicked();

private:
    Database db;
    Ui::Library* ui;
    QProcess* runningProcess;

    bool isProcessRunning() const;
    void runProcess(QString file, QString workingDirectory);
    void refreshGames();
};
