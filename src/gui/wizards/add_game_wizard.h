#pragma once

#include "../../database.h"

#include <QWizard>
#include <QtWidgets>

class InitPage;
class InfoPage;
class LastPage;

/** AddGameWizard class.
 * Class to handle overall wizard.
*/
class AddGameWizard : public QWizard
{
    Q_OBJECT
public:
    AddGameWizard(QWidget* parent = 0);
};

/** InitPage class.
 * Class to handle inital page.
*/
class InitPage : public QWizardPage
{
Q_OBJECT
public:
    InitPage(QWidget* parent = 0);
};

/** InfoPage class
 * Class to handle the page in which the user will input the information about the game they wish to add.
 */
class InfoPage : public QWizardPage
{
Q_OBJECT
public:
    InfoPage(QWidget* parent = 0);
};

/** LastPage class
 * Class to handle the last page
 */
class LastPage : public QWizardPage
{
Q_OBJECT
protected:
    void initializePage() Q_DECL_OVERRIDE;
public:
    LastPage( QWidget* parent = 0);
};