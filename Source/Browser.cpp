#include "Browser.h"
#include "ui_Browser.h"

/** Constructor for the browser object.
 * Creates the Browser object, allocates the UI and sets up a parent QWidget
 * \param parent QWidget* object to use as the parent.
 */
Browser::Browser(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::Browser)
{
    ui->setupUi(this);
    this->setObjectName("browserUI");
}

/** Destructor for the Browser object.
 * Unload the UI
 */
Browser::~Browser()
{
    delete ui;
}
