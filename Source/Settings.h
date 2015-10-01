#pragma once
#include <QtWidgets>

namespace Ui {
	class Settings;
}

class Settings : public QWidget
{
private:
	Ui::Settings *ui;

private slots:
	void on_WizardButton_clicked();

public:
	explicit Settings(QSettings* p, QWidget* parent = 0);
	~Settings();
};

