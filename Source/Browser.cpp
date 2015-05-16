#include "Browser.h"
#include "ui_Browser.h"

Browser::Browser(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::Browser)
{
    ui->setupUi(this);
    this->setObjectName("browserUI");
}

Browser::~Browser()
{
    delete ui;
}
