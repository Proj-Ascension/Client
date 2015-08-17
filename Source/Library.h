#pragma once

#include "Database.h"

#include <QWidget>
#include <QProcess>
#include <QDir>

/** Library UI namespace. */
namespace Ui
{
class Library;
}

/** Library class.
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
    void on_removeGame_clicked();

    void finished(int exitCode, QProcess::ExitStatus exitStatus);
    void onLaunchError(QProcess::ProcessError error);

private:
    Database db;
    Ui::Library* ui;
    QProcess* runningProcess;
    QList<QString> steamDirectoryList;

    bool isProcessRunning() const;
    QStringList recursiveFindFiles(QDir dir, QStringList ignoreList);
    void runProcess(QString file, QString workingDirectory);
    void runProcessWithArgs(QString file, QString workingDirectory, QString args);
    void refreshGames();
    void findSteamGames(QDir steamRoot);
    void findOriginGames(QDir originRoot);
    void findUplayGames(QDir uplayRoot);
    void parseAcf(QDir steamRoot);
};
