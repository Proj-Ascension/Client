#include "Launcher.h"
#include "ui_Launcher.h"

Launcher::Launcher(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Launcher)
{
    ui->setupUi(this);
}

Launcher::~Launcher()
{
    delete ui;
}
