#include "Settings.h"
#include "ui_Settings.h"
#include "Wizard.h"

#include <QtWidgets>
#include <QDebug>

/** Settings constructor
* Initialize the settings UI
* \param p Inherited palette configuration for setting StyleSheets.
* \param parent Pointer to parent widget.
*/
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
		connect(ui->WizardButton, SIGNAL(clicked()), QApplication::instance(), SLOT(Settings::on_WizardButton_clicked()));
}

/** Event handler for Wizard Button
*/
void Settings::on_WizardButton_clicked()
{
	Wizard* wiz = new Wizard();
	wiz->show();
}

Settings::~Settings()
{
	delete ui;
}
