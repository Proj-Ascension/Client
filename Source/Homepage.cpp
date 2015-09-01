#include "Homepage.h"
#include "ui_homepage.h"

Homepage::Homepage(QSettings* p, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::Homepage)
{
    ui->setupUi(this);
}

Homepage::~Homepage()
{
    delete ui;
}
