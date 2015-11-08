#pragma once
#include <src/database.h>
#include <src/drm/drm_all.h>

#include <boost/property_tree/ptree.hpp>

#include <vector>

namespace pt = boost::property_tree;


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
    DRMSetupWizard(QWidget* parent = 0);
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
    std::map<std::string, DRMType*> drmMap;

public:
    DRMPage(std::map<std::string, DRMType*> drmMap, QWidget *parent = 0);
    int nextId() const Q_DECL_OVERRIDE;
};

/** ResultsPage class.
 * Class to handle the results of which games have been found.
*/
class ResultsPage : public QWizardPage
{
    Q_OBJECT
    QTabWidget* tabWidget;
    QGridLayout* topLayout;
    SteamDRM* steam;
    OriginDRM* origin;
    UplayDRM* uplay;

    QPushButton* selectAllBtn;
    QPushButton* deselectAllBtn;
    QPushButton* invertBtn;

    DRMType* getCurrentDRM();

public slots:
    void tabSelected();
    void selectAll();
    void deselectAll();
    void invert();

protected:
    void initializePage() Q_DECL_OVERRIDE;

public:
    ResultsPage(std::map<std::string, DRMType*> drmMap, QWidget* parent = 0);
    int nextId() const Q_DECL_OVERRIDE;
};

/** FinalPage class.
 * Class to handle the final page.
*/
class FinalPage : public QWizardPage
{
    Q_OBJECT

protected:
    void initializePage() Q_DECL_OVERRIDE;

public:
    FinalPage(QWidget* parent = 0);
};
