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
    rss = new QSettings(QString("rss.ini"), QSettings::IniFormat);
    QVBoxLayout* newRSSVertLayout = new QVBoxLayout(this);
    QHBoxLayout* titleAndRemove = new QHBoxLayout();
    feedTitle = new QLabel();
    rssList = new QListWidget();
    deleteWidget = new QPushButton();
    deleteWidget->setText("X");
    deleteWidget->setMaximumWidth(50);
    titleAndRemove->addWidget(feedTitle);
    titleAndRemove->addWidget(deleteWidget);
    newRSSVertLayout->addLayout(titleAndRemove);
    newRSSVertLayout->addWidget(rssList);
    connect(deleteWidget, SIGNAL(clicked()), this, SLOT(deleteRSSWidget()));
    connect(rssList, SIGNAL(itemClicked(QListWidgetItem*)),this, SLOT(onListItemClicked(QListWidgetItem*)));
}

void NewsFeedWidget::addRSSItem(QString title, QString url)
{
    QListWidgetItem* item = new QListWidgetItem;
    item->setText(title);
    item->setData(32, url);
    rssList->addItem(item);
}

void NewsFeedWidget::setRSSTitle(QString feedTitle)
{
    NewsFeedWidget::feedTitle->setText(feedTitle);
}

void NewsFeedWidget::onListItemClicked(QListWidgetItem* item)
{
    QDesktopServices::openUrl(QUrl(item->data(32).toString()));
}

void NewsFeedWidget::deleteRSSWidget()
{
    rss->remove(feedTitle->text());
    delete this;
}

NewsFeedWidget::~NewsFeedWidget()
{

}