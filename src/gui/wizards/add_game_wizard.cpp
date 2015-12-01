#include "add_game_wizard.h"
#include "../../database.h"

#include <QDebug>
#include <QString>

/** AddGameWizard constructor
 * Defines the pages and initializes the database with the path given. Also sets up some window-related properties,
 * such as title and initial size.
 * \param parent Parent widget to draw from
 * \param dbPath Path to the database used
 */
AddGameWizard::AddGameWizard(QWidget* parent) : QWizard(parent)
{
    addPage(new InitPage(this));
    addPage(new InfoPage(this));
    addPage(new LastPage(this));

    setWindowTitle("Add game wizard");
}

/** InitPage constructor
 * Defines some initial properties for the initial page.
 * \param parent Parent widget to draw from
 */
InitPage::InitPage(QWidget* parent) : QWizardPage(parent)
{
    setTitle("Intro");
    QLabel* label = new QLabel("This wizard will help you easily add games to your database.");
    label->setWordWrap(true);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(label);
    setLayout(layout);
}

/** InfoPage constructor
 * Defines some initial properties for the page in which the user will input the game's information.
 * \param parent Parent widget to draw from
 */
InfoPage::InfoPage(QWidget* parent) : QWizardPage(parent)
{
    setTitle("Input game details.");
    QLineEdit* nameEdit = new QLineEdit(this);
    dirEdit = new QLineEdit(this);
    exeEdit = new QLineEdit(this);
    QLineEdit* argsEdit = new QLineEdit(this);
    QLabel* nameLabel = new QLabel("Name: ");
    QLabel* dirLabel = new QLabel("Directory: ");
    QLabel* exeLabel = new QLabel("Executable: ");
    QLabel* argsLabel = new QLabel("Arguments (optional): ");
    QPushButton* dirEditPicker = new QPushButton(this);
    QPushButton* exeEditPicker = new QPushButton(this);

    registerField("nameEdit*", nameEdit);
    registerField("dirEdit*", dirEdit);
    registerField("exeEdit*", exeEdit);
    registerField("argsEdit", argsEdit);

    connect(dirEditPicker, &QPushButton::clicked, [&]() { selectPath(this->dirEdit, QFileDialog::Directory);});
    connect(exeEditPicker, &QPushButton::clicked, [&]() { selectPath(this->exeEdit, QFileDialog::ExistingFile);});

    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(nameLabel, 0, 0);
    layout->addWidget(nameEdit, 0, 1);
    layout->addWidget(dirLabel, 1, 0);
    layout->addWidget(dirEdit, 1, 1);
    layout->addWidget(dirEditPicker, 1, 2);
    layout->addWidget(exeLabel, 2, 0);
    layout->addWidget(exeEdit, 2, 1);
    layout->addWidget(exeEditPicker, 2, 2);
    layout->addWidget(argsLabel, 3, 0);
    layout->addWidget(argsEdit, 3, 1);

    setLayout(layout);
}

/** selectPath function
 * Use values selected from the user's filesystem for the lineEdit passed.
 * \param lineEdit Widget to store the path in
 * \param fileMode File mode to use
 */
void InfoPage::selectPath(QLineEdit* lineEdit, QFileDialog::FileMode fileMode)
{
    QFileDialog dialog(this);
    dialog.setFileMode(fileMode);

    if (dialog.exec())
    {
        lineEdit->setText(dialog.selectedFiles().at(0));
    }
}

/** LastPage constructor
 * Defines some initial properties for the page in which the user will input the game's information.
 * \param parent Parent widget to draw from
 */
LastPage::LastPage(QWidget* parent) : QWizardPage(parent)
{
    setTitle("Done");
}

/** Initializes the last page. This function is called when the NextButton on the previous page is clicked,
 * which allows the state of the previous page to be transferred to this one.
 * The function checks if the game already exists in the database, and if not, adds that game.
*/
void LastPage::initializePage()
{
    QLabel* label = new QLabel(this);

    if (!std::get<0>(Database::getInstance().isExistant(field("nameEdit").toString())))
    {
        label->setText("Game added successfully.");
        Database::getInstance().addGame(field("nameEdit").toString(),field("dirEdit").toString(), field("exeEdit").toString(),field("argsEdit").toString());
    }
    else
    {
        label->setText("Game already exists.");
    }
    label->setWordWrap(true);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(label);
    setLayout(layout);
}