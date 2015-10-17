#include "Settings.h"
#include "ui_Settings.h"
#include "DRMSetupWizard.h"

/** Settings constructor
* Initialize the settings UI
* \param p Inherited palette configuration for setting StyleSheets.
* \param parent Pointer to parent widget.
*/
Settings::Settings(QSettings* p, QWidget* parent) : QWidget(parent), ui(new Ui::Settings)
{
    ui->setupUi(this);
    this->setObjectName("settingsUI");
    this->setStyleSheet( "QGroupBox{ background-color: " + p->value("Primary/SecondaryBase").toString() + ";}"
                                 "QLabel { color:  #ffffff;}"
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
    ui->ClearDatabaseButton->setFont(buttonFont);
    ui->ClearDatabaseButton->setText("Clear Database");
    ui->AccentButton->setFont(buttonFont);
    ui->AccentButton->setStyleSheet("QPushButton{ background-color: " + p->value("Accent/LightAccent").toString() + ";}");
    ui->AccentButton_2->setFont(buttonFont);
    ui->AccentButton_2->setStyleSheet("QPushButton{ background-color: " + p->value("Accent/MediumAccent").toString() + ";}");
    ui->AccentButton_3->setFont(buttonFont);
    ui->AccentButton_3->setStyleSheet("QPushButton {background-color: " + p->value("Accent/DarkAccent").toString() + ";}");
    ui->ResetAccents->setFont(buttonFont);
    //ui->label_2->setStyleSheet("{color: #FFFFFF}");
    ui->ResetAccents->setText("Reset Colors to Default");
    //ui->UserSettingsBox->setStyleSheet("color: #FFFFFF;} ");
    //ui->ClientSettingsBox->setStyleSheet("color: #FFFFFF;} ");
    //ui->StyleSettingsBox->setStyleSheet("color: #FFFFFF;} ");
	if (!db.init())
	{
		QMessageBox error;
		error.critical(0, "Error!", "An error occured while trying to load the database.");
		exit(EXIT_FAILURE);
	}
}

/** Event handler for Wizard Button
* Runs setup wizard on click
*/
void Settings::on_WizardButton_clicked()
{
	DRMSetupWizard* wiz = new DRMSetupWizard();
	wiz->show();
}

void Settings::on_AccentButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white);
    updateAccent(1,color);
}

void Settings::on_AccentButton_2_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white);
    updateAccent(2,color);
}

void Settings::on_AccentButton_3_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white);
    updateAccent(3,color);
}

void Settings::on_ResetAccents_clicked()
{
    QSettings palette(QSettings::IniFormat, QSettings::UserScope, "Project Ascension", "palette");

    if (palette.isWritable())
    {
        palette.beginGroup("Primary");

        palette.setValue("ActiveElement", "#FFFFFF");
        palette.setValue("InactiveSelection", "#686868");
        palette.setValue("HoverSelection", "#ADADAD");
        palette.setValue("DarkElement", "#4D5250");
        palette.setValue("LightText", "#FFFFFF");
        palette.setValue("DarkText", "#242424");
        palette.setValue("SubText", "#B2B4B3");
        palette.setValue("PrimaryBase", "#282828");
        palette.setValue("SecondaryBase", "#1F1F1F");
        palette.setValue("TertiaryBase", "#131313");
        palette.setValue("DarkestBase", "#0F0F0F");
        palette.endGroup();

        palette.beginGroup("Accent");

        palette.setValue("LightAccent", "#E58F12");
        palette.setValue("MediumAccent", "#895f06");
        palette.setValue("DarkAccent", "#6a4a05");
        palette.endGroup();
    }
    ui->AccentButton->setStyleSheet("QPushButton{ background-color: " + palette.value("Accent/LightAccent").toString() + ";}");
    ui->AccentButton_2->setStyleSheet("QPushButton{ background-color: " + palette.value("Accent/MediumAccent").toString() + ";}");
    ui->AccentButton_3->setStyleSheet("QPushButton{ background-color: " + palette.value("Accent/DarkAccent").toString() + ";}");
}

void Settings::updateAccent(int accent, QColor color)
{
    QSettings palette(QSettings::IniFormat, QSettings::UserScope, "Project Ascension", "palette");
    if (palette.isWritable())
    {
        palette.beginGroup("Primary");
        palette.endGroup();
        palette.beginGroup("Accent");
        if(accent == 1) palette.setValue("LightAccent", color.name());
        if(accent == 2) palette.setValue("MediumAccent", color.name());
        if(accent == 3) palette.setValue("DarkAccent", color.name());
        palette.endGroup();
    }
    if(accent == 1) ui->AccentButton->setStyleSheet("QPushButton{ background-color: " + palette.value("Accent/LightAccent").toString() + ";}");
    if(accent == 2) ui->AccentButton_2->setStyleSheet("QPushButton{ background-color: " + palette.value("Accent/MediumAccent").toString() + ";}");
    if(accent == 3) ui->AccentButton_3->setStyleSheet("QPushButton{ background-color: " + palette.value("Accent/DarkAccent").toString() + ";}");
}

void Settings::on_ClearDatabaseButton_clicked()
{
    int ret = QMessageBox(QMessageBox::Question, "Deleting Database", "Proceeding will delete the database, the database will be non-recoverable. Proceed?", QMessageBox::Yes | QMessageBox::No).exec();
    switch (ret)
    {
    case QMessageBox::Yes:
        db.reset();
        break;
    case QMessageBox::No:
        break;
    default:
        break;
    }
}

Settings::~Settings()
{
    delete ui;
}
