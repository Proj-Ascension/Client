#include "BuddyList.h"
#include "ui_BuddyList.h"

BuddyList::BuddyList(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BuddyList)
{
    ui->setupUi(this);
    this->setStyleSheet("QPushButton#close{background-image:url(:/SystemMenu/Icons/Close.png)}QPushButton:hover#close{background-image:url(:/SystemMenu/Icons/CloseHover.png)}");
}

BuddyList::~BuddyList()
{
    delete ui;
}

void BuddyList::on_close_clicked()
{
    delete ui;
}
