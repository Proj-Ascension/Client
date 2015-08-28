#pragma once

#include <QtWidgets>
#include <vector>
#include "Database.h"

class IntroPage;
class ResultsPage;
class DRMPage;
class FinalPage;

typedef std::vector<std::vector<QString>> GameList;

class Wizard : public QWizard
{
Q_OBJECT
public:
    Database db;
    enum pages { INTRO, DRM, RESULTS, FINAL };
    Wizard(QWidget* parent = 0, QString dbPath = "./");
    DRMPage* drmPage;
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
    QGridLayout* layout;

    QCheckBox* steamBox;
    QCheckBox* originBox;
    QCheckBox* uplayBox;

    QLabel* statusLabel;
    QLabel* platformLabel;
    QLabel* descLabel;

    void checkSteamExists();
    void checkUplayExists();
    void checkOriginExists();


public:
    QString steamPath;
    QString originPath;
    QString uplayPath;
    DRMPage(QWidget* parent = 0);
};

class ResultsPage : public QWizardPage
{
    Q_OBJECT

    QStringList recursiveFindFiles(QDir dir, QStringList ignoreList);
    void parseAcf(QDir steamRoot);
    QGridLayout* top_layout;
    QGridLayout* layout;
    QScrollArea* scrollArea;
    QStringList steamDirectoryList;
    Database db;

    QDir steamRoot;
    QDir uplayRoot;
    QDir originRoot;

    GameList steamVector;
    GameList originVector;
    GameList uplayVector;

protected:
    void initializePage();

public:
    ResultsPage(Database db, DRMPage& drmPage, QWidget* parent = 0);
    void findOriginGames();
    void findUplayGames();
    void findSteamGames();
};

class FinalPage : public QWizardPage
{
    Q_OBJECT
public:
    FinalPage(QWidget* parent = 0);
};