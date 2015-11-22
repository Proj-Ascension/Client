#include "settings.h"
#include "../wizards/drm_setup_wizard.h"

/** Settings constructor
* Initialize the settings UI
* \param p Inherited palette configuration for setting StyleSheets.
* \param parent Pointer to parent widget.
*/

Settings::Settings(QSettings* p, QWidget* parent) : QWidget(parent)
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
                                "QGroupBox { color: " + p->value("Primary/LightText").toString() + ";}");
    QGroupBox* userSettingsBox = new QGroupBox(this);
    vertlayout->addWidget(userSettingsBox);
    userSettingsBox->setTitle("User Settings");

    QHBoxLayout* horlayout = new QHBoxLayout();
    vertlayout->addLayout(horlayout);

    //Horizontal Layout
    //Client Settings
    QGroupBox* clientSettingsBox = new QGroupBox(this);

    horlayout->addWidget(clientSettingsBox);
    QVBoxLayout* clientBoxLayout = new QVBoxLayout(clientSettingsBox);
    clientSettingsBox->setLayout(clientBoxLayout);
    clientSettingsBox->setTitle("Client Settings");
    clientBoxLayout->setMargin(50);
    clientBoxLayout->setSpacing(5);

    QPushButton* wizardButton = new QPushButton(this);
    wizardButton->setFont(buttonFont);
    wizardButton->setText("Add Games to Ascension");
    clientBoxLayout->addWidget(wizardButton);

    QPushButton* clearDatabaseButton = new QPushButton(this);
    clearDatabaseButton->setFont(buttonFont);
    clearDatabaseButton->setText("Clear Database");
    clientBoxLayout->addWidget(clearDatabaseButton);
    //End Client Settings

    horlayout->addSpacing(10);

    //Style Settings
    QGroupBox* styleSettingsBox = new QGroupBox(this);
    horlayout->addWidget(styleSettingsBox);
    QVBoxLayout* styleBoxLayout = new QVBoxLayout(styleSettingsBox);
    styleSettingsBox->setLayout(styleBoxLayout);
    styleSettingsBox->setTitle("Style Settings");
    styleBoxLayout->setMargin(50);

    QLabel* accentLabel = new QLabel(this);
    accentLabel->setText("Accent Settings:");
    accentLabel->setStyleSheet("color: " + p->value("Primary/LightText").toString() + "; ");

    accentButton_1 = new QPushButton(this);
    accentButton_1->setFont(buttonFont);
    accentButton_1->setStyleSheet("QPushButton{ background-color: " + p->value("Accent/LightAccent").toString() + ";""}");
    accentButton_1->setText("Set Light Accent");
    accentButton_2 = new QPushButton(this);
    accentButton_2->setFont(buttonFont);
    accentButton_2->setStyleSheet("QPushButton{ background-color: " + p->value("Accent/MediumAccent").toString() + ";}");
    accentButton_2->setText("Set Medium Accent");
    accentButton_3 = new QPushButton(this);
    accentButton_3->setFont(buttonFont);
    accentButton_3->setStyleSheet("QPushButton{ background-color: " + p->value("Accent/DarkAccent").toString() + ";}");
    accentButton_3->setText("Set Dark Accent");

    QLabel* textLabel = new QLabel(this);
    textLabel->setText("Text Settings:");
    textLabel->setStyleSheet("color: " + p->value("Primary/LightText").toString() + "; ");

    textColourButton_1 = new QPushButton(this);
    textColourButton_1->setFont(buttonFont);
    textColourButton_1->setText("Set Light Text Colour");
    textColourButton_2 = new QPushButton(this);
    textColourButton_2->setFont(buttonFont);
    textColourButton_2->setText("Set Dark Text Colour");
    textColourButton_3 = new QPushButton(this);
    textColourButton_3->setFont(buttonFont);
    textColourButton_3->setText("Set Sub Text Colour");

    QPushButton* resetAccents = new QPushButton(this);
    resetAccents->setFont(buttonFont);
    resetAccents->setText("Reset Colors to Default");
    styleBoxLayout->addWidget(textLabel);

    //Add text colour buttons
    styleBoxLayout->addWidget(textColourButton_1);
    styleBoxLayout->addWidget(textColourButton_2);
    styleBoxLayout->addWidget(textColourButton_3);
    //End Add text colour buttons
    styleBoxLayout->addWidget(accentLabel);
    //Add Accent Buttons
    styleBoxLayout->addWidget(accentButton_1);
    styleBoxLayout->addWidget(accentButton_2);
    styleBoxLayout->addWidget(accentButton_3);
    styleBoxLayout->addWidget(resetAccents);
    //End Add Accent Buttons

    //End Style Settings

    horlayout->addSpacing(10);

    //Empty Group Box
    QGroupBox* testSettingsBox = new QGroupBox(this);
    horlayout->addWidget(testSettingsBox);
    //End Empty Group Box

    userSettingsBox->setStyleSheet("QGroupBox{ background-color: " + p->value("Primary/SecondaryBase").toString() + ";}");
    clientSettingsBox->setStyleSheet("QGroupBox{ background-color: " + p->value("Primary/SecondaryBase").toString() + ";}");
    styleSettingsBox->setStyleSheet("QGroupBox{ background-color: " + p->value("Primary/SecondaryBase").toString() + ";}");
    testSettingsBox->setStyleSheet("QGroupBox{ background-color: " + p->value("Primary/SecondaryBase").toString() + ";}");

    //End Horizontal Layout

    connect(wizardButton, SIGNAL(clicked()), this, SLOT(rerunGameWizard()));
    connect(clearDatabaseButton, SIGNAL(clicked()), this, SLOT(clearDatabase()));
    connect(accentButton_1, SIGNAL(clicked()), this, SLOT(setLightAccent()));
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
    accentButton_1->setStyleSheet("QPushButton{ background-color: " + palette.value("Accent/LightAccent").toString() + ";}");
    accentButton_2->setStyleSheet("QPushButton{ background-color: " + palette.value("Accent/MediumAccent").toString() + ";}");
    accentButton_3->setStyleSheet("QPushButton{ background-color: " + palette.value("Accent/DarkAccent").toString() + ";}");
}

void Settings::updateAccent(int accent, QColor color)
{
    if (!color.isValid())
    {
        return;
    }

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
    if(accent == 1) accentButton_1->setStyleSheet("QPushButton{ background-color: " + palette.value("Accent/LightAccent").toString() + ";}");
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
