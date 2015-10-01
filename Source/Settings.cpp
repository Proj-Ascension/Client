#include "Settings.h"
#include "ui_Settings.h"
#include <QtWidgets>

Settings::Settings(QWidget* parent) : QWidget(parent),ui(new Ui::Settings)
{
		ui->setupUi(this);
		this->setObjectName("communityUI");
		this->setStyleSheet("background-color: " + p->value("Primary/SecondaryBase").toString() + ";} "
			"QPushButton {"
			"color: " + p->value("Primary/LightText").toString() + "; "
			"background-color: " + p->value("Primary/DarkElement").toString() + "; "
			"border: none; margin: 0px; padding: 0px;} "
			"QPushButton:hover {"
			"background-color: " + p->value("Primary/InactiveSelection").toString() + ";} "
			"color: " + p->value("Primary/LightText").toString() + ";");
		QFont buttonFont("SourceSansPro", 12);

}


Settings::~Settings()
{
}
