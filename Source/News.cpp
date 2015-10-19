#include "News.h"
#include "ui_News.h"
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
News::News(QSettings* p, QWidget* parent) : QWidget(parent)
{
    QVBoxLayout* newsTabLayout = new QVBoxLayout(this);

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
    QHBoxLayout* addRSSLayout = new QHBoxLayout();
    rssAddress = new QLineEdit();
    addRSSLayout->addWidget(rssAddress);
    QPushButton* setRSS = new QPushButton();
    setRSS->setText("Add RSS");
    addRSSLayout->addWidget(setRSS);
    horLayout = new QHBoxLayout();
    newsTabLayout->addLayout(addRSSLayout);
    newsTabLayout->addLayout(horLayout);
    loadFeeds();
    connect(setRSS, SIGNAL(clicked()), this, SLOT(setRSSFeed()));
}

void News::setRSSFeed()
{
    QString url = rssAddress->text();
    getRSSFeed(url);
}

void News::getRSSFeed(QString url)
{
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    reply = manager.get(request);
    connect(reply, SIGNAL(finished()),this, SLOT(onRSSReturned()));
}

void News::onRSSReturned()
{
    NewsFeedWidget* newsFeedWidget = new NewsFeedWidget(this);
    QByteArray data = reply->readAll();
    QXmlStreamReader xml(data);
    while(!xml.atEnd())
    {
        if(xml.isStartElement()) {
            if (xml.name() == "channel")
            {
                xml.readNext();
                if (xml.name() == "title") {
                    QString title = xml.readElementText();
                    newsFeedWidget->setRSSTitle(title);
                    saveFeeds(title, rssAddress->text());
                }
            }
            if (xml.name() == "item")
            {
                xml.readNext();
                newsFeedWidget->addRSSItem(xml.readElementText());
            }
        }
        xml.readNext();
    }
    xml.clear();
    horLayout->addWidget(newsFeedWidget);

}

void News::saveFeeds(QString title, QString url)
{
    QFile file(QCoreApplication::applicationDirPath() + "/rssFeeds.xml");
    bool feedsExist = file.exists();
    if(feedsExist)
    {
        file.open(QIODevice::Append);
    }
    else
    {
        file.open(QIODevice::WriteOnly);
    }
    QXmlStreamWriter xmlWriter(&file);
    if(!feedsExist) xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("item");
        xmlWriter.writeTextElement("title", title);
        xmlWriter.writeTextElement("url", url);
        xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();
    file.close();
}

void News::loadFeeds()
{
    QFile file(QCoreApplication::applicationDirPath() + "/rssFeeds.xml");
    if(file.exists())
    {
        file.open(QIODevice::ReadOnly);
        QXmlStreamReader xmlReader(&file);
        qDebug() << file.readAll();
        /*while(!xmlReader.isEndDocument())
        {
        }*/
        file.close();
        xmlReader.clear();
    }
}

News::~News()
{
}
