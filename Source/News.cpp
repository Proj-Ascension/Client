#include "News.h"
#include "NewsFeedWidget.h"

#include <QtWidgets>
#include <QDebug>
#include <QSettings>
#include <iostream>
#include <thread>

/** Settings constructor
* Initialize the news UI
* \param p Inherited palette configuration for setting StyleSheets.
* \param parent Pointer to parent widget.
*/
News::News(QSettings* p, QWidget* parent) : QWidget(parent)
{
    this->setStyleSheet("QListWidget { background-color: " + p->value("Primary/SecondaryBase").toString() + ";} "
                        "QListWidget { color: " + p->value("Primary/LightText").toString() + "; }"
                        "QLabel { color: " + p->value("Primary/LightText").toString() + "; }"
                        "QPushButton {"
                        "color: " + p->value("Primary/LightText").toString() + "; "
                        "background-color: " + p->value("Primary/DarkElement").toString() + "; "
                        "border: none; margin: 0px; padding: 0px;} "
                        "QPushButton:hover {"
                        "background-color: " + p->value("Primary/InactiveSelection").toString() + ";} "
                        "color: " + p->value("Primary/LightText").toString() + ";");
    QFont buttonFont("SourceSansPro", 9);

    rss = new QSettings (QString("rss.ini"), QSettings::IniFormat);
    manager = new QNetworkAccessManager(this);

    QVBoxLayout* newsTabLayout = new QVBoxLayout(this);

    QHBoxLayout* addRSSLayout = new QHBoxLayout();
    rssAddress = new QLineEdit(this);
    addRSSLayout->addWidget(rssAddress);
    QPushButton* setRSS = new QPushButton();
    setRSS->setText("Add RSS");
    addRSSLayout->addWidget(setRSS);
    newsTabLayout->addLayout(addRSSLayout);

    hNewsLayout = new QHBoxLayout();
    newsTabLayout->addLayout(hNewsLayout);

    loadFeeds();

    connect(setRSS, SIGNAL(clicked()), this, SLOT(setRSSFeed()));
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onRSSReturned(QNetworkReply*)));
}

News::~News()
{
}

void News::setRSSFeed()
{
    QString url = rssAddress->text();
    getRSSFeed(url);
}

void News::getRSSFeed(QString url)
{
    qDebug() << "Getting RSS feed of:" << url;
    QNetworkRequest request(url);
    manager->get(request);
}

void News::onRSSReturned(QNetworkReply* reply)
{
    NewsFeedWidget* newsFeedWidget = new NewsFeedWidget(this);
    QByteArray data = reply->readAll();
    QXmlStreamReader xml(data);
    while (!xml.atEnd())
    {
        if (xml.isStartElement()) {
            if (xml.name() == "channel")
            {
                xml.readNext();
                if (xml.name() == "title")
                {
                    QString title = xml.readElementText();
                    newsFeedWidget->setRSSTitle(title);
                    //rss->beginGroup("feeds");
                    if (!rss->contains(title))
                    {
                        qDebug() << title;
                        saveFeeds(title, reply->url().toString());
                    }
                }
            }
            if (xml.name() == "item")
            {
                QString url;
                QString title;
                xml.readNext();
                if(xml.name() == "title")title = xml.readElementText();
                xml.readNext();
                if(xml.name() == "link")url = xml.readElementText();
                newsFeedWidget->addRSSItem(title, url);
            }
            /*if(xml.name() == "link")
            {
                //xml.readNext();
                qDebug() << xml.readElementText();
                //newsFeedWidget->addRSSItem(xml.readElementText());
            }*/
        }
        xml.readNext();
    }
    xml.clear();
    hNewsLayout->addWidget(newsFeedWidget);
    reply->close();
}

void News::saveFeeds(QString title, QString url)
{
    qDebug() << "Saving rss feed" << title << url;
    if (rss->isWritable())
    {
        //rss->beginGroup("feeds");
        rss->setValue(title, url);
    }
    qDebug() << rss->allKeys();
}

void News::loadFeeds()
{
    QStringList childKeys = rss->allKeys();
    for (int i = 0; i < childKeys.length(); i++)
    {
        QString url = rss->value(childKeys.value(i)).toString();
        getRSSFeed(url);
    }
}
