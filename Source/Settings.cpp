#include "Settings.h"
#include "DRMSetupWizard.h"

#include <QLayout>
#include <QGroupBox>

/** Settings constructor
* Initialize the settings UI
* \param p Inherited palette configuration for setting StyleSheets.
* \param parent Pointer to parent widget.
*/

Settings::Settings(QSettings* p, QWidget* parent) : QWidget(parent) //,ui(new Ui::Settings)
{
    QFont buttonFont("SourceSansPro", 9);
    QVBoxLayout* vertlayout = new QVBoxLayout(this);
    this->setLayout(vertlayout);
    this->setStyleSheet("QPushButton {"
                                "color: " + p->value("Primary/LightText").toString() + "; "
                                "background-color: " + p->value("Primary/DarkElement").toString() + "; "
                                "border: none;}"
                                "QPushButton:hover {"
                                "background-color: " + p->value("Primary/InactiveSelection").toString() + ";} "
                                "color: " + p->value("Primary/LightText").toString() + ";");
    QGroupBox* userSettingsBox = new QGroupBox();
    vertlayout->addWidget(userSettingsBox);

    QHBoxLayout* horlayout = new QHBoxLayout(this);
    vertlayout->addLayout(horlayout);

    //Client Settings
    QGroupBox* clientSettingsBox = new QGroupBox();
    horlayout->addWidget(clientSettingsBox);
    QVBoxLayout* clientBoxLayout = new QVBoxLayout();
    clientSettingsBox->setLayout(clientBoxLayout);

    QPushButton* wizardButton = new QPushButton();
    wizardButton->setFont(buttonFont);
    wizardButton->setText("Add Games to Ascension");
    clientBoxLayout->addWidget(wizardButton);

    QPushButton* clearDatabaseButton = new QPushButton();
    clearDatabaseButton->setFont(buttonFont);
    clearDatabaseButton->setText("Clear Database");
    clientBoxLayout->addWidget(clearDatabaseButton);

    //End Client Settings

    //Style Settings
    QGroupBox* styleSettingsBox = new QGroupBox();
    horlayout->addWidget(styleSettingsBox);
    QVBoxLayout* styleBoxLayout = new QVBoxLayout();
    styleSettingsBox->setLayout(styleBoxLayout);

    accentButton = new QPushButton();
    accentButton->setFont(buttonFont);
    accentButton->setStyleSheet("QPushButton{ background-color: " + p->value("Accent/LightAccent").toString() + ";}");
    accentButton_2 = new QPushButton();
    accentButton_2->setFont(buttonFont);
    accentButton_2->setStyleSheet("QPushButton{ background-color: " + p->value("Accent/MediumAccent").toString() + ";}");
    accentButton_3 = new QPushButton();
    accentButton_3->setFont(buttonFont);
    accentButton_3->setStyleSheet("QPushButton{ background-color: " + p->value("Accent/DarkAccent").toString() + ";}");
    QPushButton* resetAccents = new QPushButton();
    resetAccents->setFont(buttonFont);
    resetAccents->setText("Reset Colors to Default");


    styleBoxLayout->addWidget(accentButton);
    styleBoxLayout->addWidget(accentButton_2);
    styleBoxLayout->addWidget(accentButton_3);
    styleBoxLayout->addWidget(resetAccents);
    //End Style Settings

    QGroupBox* testSettingsBox = new QGroupBox();
    horlayout->addWidget(testSettingsBox);

    userSettingsBox->setStyleSheet("QGroupBox{ background-color: " + p->value("Primary/SecondaryBase").toString() + ";}");
    clientSettingsBox->setStyleSheet("QGroupBox{ background-color: " + p->value("Primary/SecondaryBase").toString() + ";}");
    styleSettingsBox->setStyleSheet("QGroupBox{ background-color: " + p->value("Primary/SecondaryBase").toString() + ";}");
    testSettingsBox->setStyleSheet("QGroupBox{ background-color: " + p->value("Primary/SecondaryBase").toString() + ";}");

    connect(wizardButton, SIGNAL(clicked()), this, SLOT(rerunGameWizard()));
    connect(clearDatabaseButton, SIGNAL(clicked()), this, SLOT(clearDatabase()));
    connect(accentButton, SIGNAL(clicked()), this, SLOT(setLightAccent()));
    connect(accentButton_2, SIGNAL(clicked()), this, SLOT(setMediumAccent()));
    connect(accentButton_3, SIGNAL(clicked()), this, SLOT(setDarkAccent()));
    connect(resetAccents, SIGNAL(clicked()), this, SLOT(resetAccents()));

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
void Settings::rerunGameWizard()
{
	DRMSetupWizard* wiz = new DRMSetupWizard();
	wiz->show();
}

void Settings::setLightAccent()
{
    QColor color = QColorDialog::getColor(Qt::white);
    updateAccent(1,color);
}

void Settings::setMediumAccent()
{
    QColor color = QColorDialog::getColor(Qt::white);
    updateAccent(2,color);
}

void Settings::setDarkAccent()
{
    QColor color = QColorDialog::getColor(Qt::white);
    updateAccent(3,color);
}

void Settings::resetAccents()
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
    accentButton->setStyleSheet("QPushButton{ background-color: " + palette.value("Accent/LightAccent").toString() + ";}");
    accentButton_2->setStyleSheet("QPushButton{ background-color: " + palette.value("Accent/MediumAccent").toString() + ";}");
    accentButton_3->setStyleSheet("QPushButton{ background-color: " + palette.value("Accent/DarkAccent").toString() + ";}");
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
    if(accent == 1) accentButton->setStyleSheet("QPushButton{ background-color: " + palette.value("Accent/LightAccent").toString() + ";}");
    if(accent == 2) accentButton_2->setStyleSheet("QPushButton{ background-color: " + palette.value("Accent/MediumAccent").toString() + ";}");
    if(accent == 3) accentButton_3->setStyleSheet("QPushButton{ background-color: " + palette.value("Accent/DarkAccent").toString() + ";}");
}

void Settings::clearDatabase()
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
}
