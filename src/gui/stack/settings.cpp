#include <src/gui/dialogs/ascension_dialog.h>
#include "settings.h"
#include "../wizards/drm_setup_wizard.h"

/** Settings constructor
* Initialize the settings UI
* \param p Inherited palette configuration for setting StyleSheets.
* \param parent Pointer to parent widget.
*/

Settings::Settings(QSettings* p, QWidget* parent) : QWidget(parent)
{
    this->setStyleSheet("QGroupBox {color: " + p->value("Primary/LightText").toString() + ";"
                        "background-color: " + p->value("Primary/SecondaryBase").toString() + ";}");

    // Main Vertical Layout
    QVBoxLayout* vertlayout = new QVBoxLayout(this);

    // User Settings Group Box
    QGroupBox* userSettingsBox = new QGroupBox(this);
    vertlayout->addWidget(userSettingsBox);
    userSettingsBox->setTitle("User Settings");

    // Client and Style Horizontal Layout
    QHBoxLayout* horlayout = new QHBoxLayout();
    vertlayout->addLayout(horlayout);

    // Client Settings
    QGroupBox* clientSettingsBox = new QGroupBox(this);
    clientSettingsBox->setTitle("Client Settings");
    horlayout->addWidget(clientSettingsBox);

    // Client Settings - Vertical Layout
    QVBoxLayout* clientBoxLayout = new QVBoxLayout(clientSettingsBox);
    clientBoxLayout->setMargin(50);
    clientBoxLayout->setSpacing(5);
    clientSettingsBox->setLayout(clientBoxLayout);

    QPushButton* wizardButton = new QPushButton(clientSettingsBox);
    wizardButton->setText("Add Games to Ascension");
    clientBoxLayout->addWidget(wizardButton);
    connect(wizardButton, SIGNAL(clicked()), this, SLOT(rerunGameWizard()));

    QPushButton* clearDatabaseButton = new QPushButton(clientSettingsBox);
    clearDatabaseButton->setText("Clear Database");
    clientBoxLayout->addWidget(clearDatabaseButton);
    connect(clearDatabaseButton, SIGNAL(clicked()), this, SLOT(clearDatabase()));

    horlayout->addSpacing(10);

    // Style Settings
    QGroupBox* styleSettingsBox = new QGroupBox(this);
    styleSettingsBox->setTitle("Style Settings");
    horlayout->addWidget(styleSettingsBox);

    // Style Settings - Vertical Layout
    QVBoxLayout* styleBoxLayout = new QVBoxLayout(styleSettingsBox);
    styleBoxLayout->setMargin(50);
    styleSettingsBox->setLayout(styleBoxLayout);

    QLabel* accentLabel = new QLabel(styleSettingsBox);
    accentLabel->setText("Accent Settings:");
    accentLabel->setStyleSheet("color: " + p->value("Primary/LightText").toString() + "; ");
    styleBoxLayout->addWidget(accentLabel);

    accentButton_1 = new QPushButton(styleSettingsBox);
    accentButton_1->setStyleSheet("QPushButton {background-color: " + p->value("Accent/LightAccent").toString() + ";}");
    accentButton_1->setText("Set Light Accent");
    styleBoxLayout->addWidget(accentButton_1);

    accentButton_2 = new QPushButton(styleSettingsBox);
    accentButton_2->setStyleSheet("QPushButton {background-color: " + p->value("Accent/MediumAccent").toString() + ";}");
    accentButton_2->setText("Set Medium Accent");
    styleBoxLayout->addWidget(accentButton_2);

    accentButton_3 = new QPushButton(styleSettingsBox);
    accentButton_3->setStyleSheet("QPushButton {background-color: " + p->value("Accent/DarkAccent").toString() + ";}");
    accentButton_3->setText("Set Dark Accent");
    styleBoxLayout->addWidget(accentButton_3);

    QLabel* textLabel = new QLabel(styleSettingsBox);
    textLabel->setText("Text Settings:");
    textLabel->setStyleSheet("color: " + p->value("Primary/LightText").toString() + "; ");
    styleBoxLayout->addWidget(textLabel);

    textColourButton_1 = new QPushButton(styleSettingsBox);
    textColourButton_1->setText("Set Light Text Colour");
    styleBoxLayout->addWidget(textColourButton_1);
    connect(accentButton_1, SIGNAL(clicked()), this, SLOT(setLightAccent()));

    textColourButton_2 = new QPushButton(styleSettingsBox);
    textColourButton_2->setText("Set Dark Text Colour");
    styleBoxLayout->addWidget(textColourButton_2);
    connect(accentButton_2, SIGNAL(clicked()), this, SLOT(setMediumAccent()));

    textColourButton_3 = new QPushButton(styleSettingsBox);
    textColourButton_3->setText("Set Sub Text Colour");
    styleBoxLayout->addWidget(textColourButton_3);
    connect(accentButton_3, SIGNAL(clicked()), this, SLOT(setDarkAccent()));

    styleBoxLayout->addSpacing(10);

    QPushButton* resetAccents = new QPushButton(styleSettingsBox);
    resetAccents->setText("Reset Colors to Default");
    styleBoxLayout->addWidget(resetAccents);
    connect(resetAccents, SIGNAL(clicked()), this, SLOT(resetAccents()));
}

Settings::~Settings()
{
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
    updateAccent(1, color);
}

void Settings::setMediumAccent()
{
    QColor color = QColorDialog::getColor(Qt::white);
    updateAccent(2, color);
}

void Settings::setDarkAccent()
{
    QColor color = QColorDialog::getColor(Qt::white);
    updateAccent(3, color);
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

    accentButton_1->setStyleSheet("QPushButton {background-color: " + palette.value("Accent/LightAccent").toString() + ";}");
    accentButton_2->setStyleSheet("QPushButton {background-color: " + palette.value("Accent/MediumAccent").toString() + ";}");
    accentButton_3->setStyleSheet("QPushButton {background-color: " + palette.value("Accent/DarkAccent").toString() + ";}");
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
        if (accent == 1) palette.setValue("LightAccent", color.name());
        if (accent == 2) palette.setValue("MediumAccent", color.name());
        if (accent == 3) palette.setValue("DarkAccent", color.name());
        palette.endGroup();
    }
    if (accent == 1) accentButton_1->setStyleSheet("QPushButton {background-color: " + palette.value("Accent/LightAccent").toString() + ";}");
    if (accent == 2) accentButton_2->setStyleSheet("QPushButton {background-color: " + palette.value("Accent/MediumAccent").toString() + ";}");
    if (accent == 3) accentButton_3->setStyleSheet("QPushButton {background-color: " + palette.value("Accent/DarkAccent").toString() + ";}");
}

void Settings::clearDatabase()
{
    bool ret = AscensionDialog::showConfirmDialog("Deleting Database", "Proceeding will delete the database, the database will be non-recoverable.\nProceed?");
    if (!ret)
    {
        Database::getInstance().reset();
    }
}

