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
	void onLaunchError(QProcess::ProcessError error);

	void on_removeGame_clicked();

private:
	Database db;
	Ui::Library* ui;
	QProcess* runningProcess;
	QList<QString> steamDirectoryList;

	bool isProcessRunning() const;
	void runProcess(QString file, QString workingDirectory);
	void refreshGames();
	void findSteamGames();
	void findUplayGames();
	void parseAcf();
};
