#include "homepage.h"
#include "ui_homepage.h"

HomePage::HomePage(QSettings* p, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::HomePage)
{
    ui->setupUi(this);
}

HomePage::~HomePage()
{
    delete ui;
}
