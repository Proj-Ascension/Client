#pragma once

#include <QWizard>
#include <vector>
#include "Database.h"

class Wizard : public QWizard
{
    Q_OBJECT
public:
    Database db;
    enum pages { INTRO, DRM, RESULTS, FINAL };
    Wizard(QWidget* parent = 0, QString dbPath = "./");
};

class IntroPage : public QWizardPage
{
    Q_OBJECT
public:
    IntroPage(QWidget* parent = 0);
};

class DRMPage : public QWizardPage
{
    Q_OBJECT

public:
    DRMPage(QWidget* parent = 0);
};

class ResultsPage : public QWizardPage
{
    Q_OBJECT

    void findOriginGames(QDir originRoot);
    void findUplayGames(QDir uplayRoot);
    void findSteamGames(QDir steamRoot);

    void parseAcf(QDir steamRoot);
    QGridLayout* top_layout;
    QGridLayout* layout;
    QScrollArea* scrollArea;
    QStringList steamDirectoryList;
    Database db;

    std::vector<std::vector<QString>> steamVector;
    std::vector<std::vector<QString>> originVector;
    std::vector<std::vector<QString>> uplayVector;

protected:
    void initializePage();

public:
    ResultsPage(Database db, QWidget* parent = 0);
};

class FinalPage : public QWizardPage
{
    Q_OBJECT
public:
    FinalPage(QWidget* parent = 0);
};
