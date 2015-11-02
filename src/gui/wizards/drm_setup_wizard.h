#pragma once
#include "../../database.h"

#include <QtWidgets>
#include <boost/property_tree/ptree.hpp>

#include <vector>

namespace pt = boost::property_tree;

class IntroPage;
class ResultsPage;
class DRMPage;
class FinalPage;

/** Pages enum, makes state transfer possible */
enum pages
{
    INTRO, /**< First page the user navigates to */
    DRM, /**< Page showing the results of whether Steam, Origin and Uplay are installed */
    RESULTS, /**< Page showing all the found games */
    FINAL /**< Games are added to the db and a message is displayed to the user */
};

/** DRMSetupWizard class.
 * Class to handle overall wizard.
*/
class DRMSetupWizard : public QWizard
{
    Q_OBJECT
public:
    Database db;
    DRMSetupWizard(QWidget* parent = 0, QString dbPath = "./");
    DRMPage* drmPage;
    ResultsPage* resultsPage;
    FinalPage* finalPage;
};

/** IntroPage class.
 * Class to handle the initial page.
*/
class IntroPage : public QWizardPage
{
    Q_OBJECT
public:
    IntroPage(QWidget* parent = 0);
    int nextId() const Q_DECL_OVERRIDE;
};

/** DRMPage class.
 * Class to handle the results of whether Steam, Origin and Uplay have been discovered..
*/
class DRMPage : public QWizardPage
{
    Q_OBJECT
    QGridLayout* layout;

    QCheckBox* steamBox;
    QCheckBox* uplayBox;

    QLabel* statusLabel;
    QLabel* platformLabel;
    QLabel* descLabel;
    QStringList steamDirectoryList;

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

/** ResultsPage class.
 * Class to handle the results of which games have been found.
*/
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

/** FinalPage class.
 * Class to handle the final page.
*/
class FinalPage : public QWizardPage
{
    Q_OBJECT
    Database db;

protected:
    void initializePage() Q_DECL_OVERRIDE;

public:
    FinalPage(Database db, QWidget* parent = 0);
};
