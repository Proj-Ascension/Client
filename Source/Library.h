#pragma once

#include "Database.h"

#include <QWidget>
#include <QProcess>

namespace Ui
{
    class Library;
}

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

private:
    Database db;
    Ui::Library* ui;
    QProcess* runningProcess;
    bool processRunning;

    void runProcess(QString file);
};
