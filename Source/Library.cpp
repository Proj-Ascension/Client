#include "Library.h"
#include "ui_Library.h"

Library::Library(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Library)
{
    ui->setupUi(this);
    this->setObjectName("libraryUI");
}

Library::~Library()
{
    delete ui;
}
