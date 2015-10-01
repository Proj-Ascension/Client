#pragma once
#include <QtWidgets>

namespace Ui {
	class Settings;
}

class Settings : public QWidget
{
private:
	Ui::Settings *ui;

public:
	Settings(QWidget* parent = 0);
	~Settings();
};

