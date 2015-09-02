#include "Community.h"
#include "ui_Community.h"
#include "BuddyList.h"

#include <QSettings>

Community::Community(QSettings* p, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::Community)
{
    ui->setupUi(this);
    this->setObjectName("communityUI");
    this->setStyleSheet("QPushButton {"
                        "color: " + p->value("Primary/LightText").toString() + "; "
                        "background-color: " + p->value("Primary/DarkElement").toString() + "; "
                        "border: none; margin: 0px; padding: 0px;} "
                        "QPushButton:hover {"
                        "background-color: " + p->value("Primary/InactiveSelection").toString() + ";} "
                        "color: " + p->value("Primary/LightText").toString() + ";");
    QFont buttonFont("SourceSansPro", 12);
    ui->buddyButton->setFont(buttonFont);
    ui->buddyButton->setText("Friends");
}

Community::~Community()
{
    delete ui;
}

void Community::on_buddyButton_clicked()
{
    QWidget *buddyL = new BuddyList();
    buddyL->show();
}
