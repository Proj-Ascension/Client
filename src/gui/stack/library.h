#pragma once

#include "../../database.h"

#include <QWidget>
#include <QProcess>
#include <QSettings>
#include <QDir>

/** Library class.
 * Class to handle the library section of the launcher 
*/
class Library : public QWidget
{
    Q_OBJECT

public:
    Library(QSettings* p, QWidget* parent = 0);
    ~Library();

private slots:
    void onAddGameClicked();
    void onRemoveGameClicked();
    void onLaunchGameClicked();
    void refreshGames();
    void finished(int exitCode, QProcess::ExitStatus exitStatus);
    void onLaunchError(QProcess::ProcessError error);

private:
    QListWidget* sidebarGameList;
    QProcess* runningProcess;

    bool isProcessRunning() const;
    void runProcess(QString file, QString workingDirectory);
    void runProcessWithArgs(QString file, QString workingDirectory, QString args);
};
