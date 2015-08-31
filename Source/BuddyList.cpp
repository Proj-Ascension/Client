#include "BuddyList.h"
#include "ui_BuddyList.h"

BuddyList::BuddyList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BuddyList)
{
    ui->setupUi(this);
}

BuddyList::~BuddyList()
{
    delete ui;
}
