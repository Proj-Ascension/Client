#pragma once

#include <QWizard>
#include "Database.h"

class Wizard : public QWizard
{
    Q_OBJECT
public:
    Database db;
    enum pages { INTRO, DRM, FINAL };
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
    QStringList steamDirectoryList;
    Database db;
public:
    DRMPage(Database, QWidget* parent = 0);
    QStringList findSteamGames(QDir steamRoot);
    QStringList parseAcf(QDir steamRoot);
    QStringList findOriginGames(QDir originRoot);
    QStringList findUplayGames(QDir uplayRoot);
};

class ResultsPage : public QWizardPage
{
    Q_OBJECT
public:
    ResultsPage(QWidget* parent = 0);
};

class FinalPage : public QWizardPage
{
    Q_OBJECT
public:
    FinalPage(QWidget* parent = 0);
};
