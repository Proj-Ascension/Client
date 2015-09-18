#pragma once

#include <QtWidgets>
#include <vector>
#include "Database.h"
#include <boost/property_tree/ptree.hpp>

namespace pt = boost::property_tree;

class IntroPage;
class ResultsPage;
class DRMPage;
class FinalPage;

enum pages { INTRO, DRM, RESULTS, FINAL };

class Wizard : public QWizard
{
Q_OBJECT
public:
    Database db;
    Wizard(QWidget* parent = 0, QString dbPath = "./");
    DRMPage* drmPage;
    ResultsPage* resultsPage;
    FinalPage* finalPage;
};

class IntroPage : public QWizardPage
{
    Q_OBJECT
public:
    IntroPage(QWidget* parent = 0);
    int nextId() const Q_DECL_OVERRIDE;
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
    int nextId() const Q_DECL_OVERRIDE;
};

class ResultsPage : public QWizardPage
{
    Q_OBJECT

    QStringList recursiveFindFiles(QDir dir);
    void parseAcf(QDir steamRoot);

    QWidget* steamViewport;
    QWidget* originViewport;
    QWidget* uplayViewport;
    QTabWidget* tabWidget;
    QGridLayout* top_layout;
    QButtonGroup* btnGroup;
    QGridLayout* steamLayout;
    QGridLayout* originLayout;
    QGridLayout* uplayLayout;
    QScrollArea* steamScrollArea;
    QScrollArea* originScrollArea;
    QScrollArea* uplayScrollArea;
    QStringList steamDirectoryList;
    Database db;

    QDir steamRoot;
    QDir uplayRoot;
    QDir originRoot;

    GameList steamVector;
    pt::ptree originTree;
    pt::ptree uplayTree;

    void printTree(boost::property_tree::ptree& pt, int level);

public slots:
    void tabSelected();
    void selectAll();
    void deselectAll();
    void invert();

protected:
    void initializePage() Q_DECL_OVERRIDE;

public:
    ResultsPage(Database db, DRMPage& drmPage, QWidget* parent = 0);
    void findOriginGames();
    void findUplayGames();
    void findSteamGames();
    int nextId() const Q_DECL_OVERRIDE;
};

class FinalPage : public QWizardPage
{
    Q_OBJECT
    Database db;

protected:
    void initializePage() Q_DECL_OVERRIDE;

public:
    FinalPage(Database db, QWidget* parent = 0);
};