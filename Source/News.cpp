#include "News.h"
#include "ui_News.h"

#include <QtWidgets>
#include <QDebug>
#include <QSettings>


/** Settings constructor
* Initialize the news UI
* \param p Inherited palette configuration for setting StyleSheets.
* \param parent Pointer to parent widget.
*/
News::News(QSettings* p, QWidget* parent) : QWidget(parent), ui(new Ui::News)
{
    ui->setupUi(this);
    this->setObjectName("newsUI");
    this->setStyleSheet("background-color: " + p->value("Primary/SecondaryBase").toString() + ";} "
            "QPushButton {"
            "color: " + p->value("Primary/LightText").toString() + "; "
                                "background-color: " + p->value("Primary/DarkElement").toString() + "; "
                                "border: none; margin: 0px; padding: 0px;} "
                                "QPushButton:hover {"
                                "background-color: " + p->value("Primary/InactiveSelection").toString() + ";} "
                                "color: " + p->value("Primary/LightText").toString() + ";");
    QFont buttonFont("SourceSansPro", 9);
}

News::~News()
{
    delete ui;
}
