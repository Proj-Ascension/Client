#include "NewsFeedWidget.h"

#include <QtWidgets>
#include <QDebug>
#include <QSettings>
#include <iostream>

/** Settings constructor
* Initialize the news UI
* \param p Inherited palette configuration for setting StyleSheets.
* \param parent Pointer to parent widget.
*/
NewsFeedWidget::NewsFeedWidget(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout* newRSSVertLayout = new QVBoxLayout(this);
    feedTitle = new QLabel();
    rssList = new QListWidget();
    newRSSVertLayout->addWidget(feedTitle);
    newRSSVertLayout->addWidget(rssList);
}

void NewsFeedWidget::addRSSItem(QString title)
{
    rssList->addItem(title);
}

void NewsFeedWidget::setRSSTitle(QString feedTitle)
{
    NewsFeedWidget::feedTitle->setText(feedTitle);
}

NewsFeedWidget::~NewsFeedWidget()
{

}