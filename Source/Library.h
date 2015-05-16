#pragma once

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
    explicit Library(QWidget* parent = 0);
    ~Library();

private slots:
    void on_testLaunch_clicked();
    void finished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    Ui::Library* ui;
    QProcess* runningProcess;
    bool processRunning;

    void runProcess(QString file);
};
