#include "Settings.h"
#include "ui_Settings.h"
#include "Wizard.h"

#include <QtWidgets>
#include <QDebug>

Settings::Settings(QSettings* p, QWidget* parent) : QWidget(parent), ui(new Ui::Settings)
{
		ui->setupUi(this);
		this->setObjectName("settingsUI");
		this->setStyleSheet("background-color: " + p->value("Primary/SecondaryBase").toString() + ";} "
			"QPushButton {"
			"color: " + p->value("Primary/LightText").toString() + "; "
			"background-color: " + p->value("Primary/DarkElement").toString() + "; "
			"border: none; margin: 0px; padding: 0px;} "
			"QPushButton:hover {"
			"background-color: " + p->value("Primary/InactiveSelection").toString() + ";} "
			"color: " + p->value("Primary/LightText").toString() + ";");
		QFont buttonFont("SourceSansPro", 9);
		ui->WizardButton->setFont(buttonFont);
		ui->WizardButton->setText("Add Games to Ascension");
}

void Settings::on_WizardButton_clicked()
{

}

Settings::~Settings()
{
	delete ui;
}
