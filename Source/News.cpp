
#include "News.h"
#include "ui_News.h"

#include <QtWidgets>
#include <QDebug>
#include <QSettings>
#include <iostream>

class rssAddress;

/** Settings constructor
* Initialize the news UI
* \param p Inherited palette configuration for setting StyleSheets.
* \param parent Pointer to parent widget.
*/
News::News(QSettings* p, QWidget* parent) : QWidget(parent)
{
    QVBoxLayout* newsTabLayout = new QVBoxLayout(this);

    this->setStyleSheet("QListWidget { background-color: " + p->value("Primary/SecondaryBase").toString() + ";} "
            "QPushButton {"
            "color: " + p->value("Primary/LightText").toString() + "; "
                                "background-color: " + p->value("Primary/DarkElement").toString() + "; "
                                "border: none; margin: 0px; padding: 0px;} "
                                "QPushButton:hover {"
                                "background-color: " + p->value("Primary/InactiveSelection").toString() + ";} "
                                "color: " + p->value("Primary/LightText").toString() + ";");
    QFont buttonFont("SourceSansPro", 9);
    RSSList = new QListWidget(this);
    RSSList->setStyleSheet("color: " + p->value("Primary/LightText").toString() + "; ");
    RSSListLabel = new QLabel();
    RSSListLabel->setStyleSheet("color: " + p->value("Primary/LightText").toString() + "; ");

    QHBoxLayout* addRSSLayout = new QHBoxLayout();
    rssAddress = new QLineEdit();
    addRSSLayout->addWidget(rssAddress);
    QPushButton* setRSS = new QPushButton();
    setRSS->setText("Set RSS");
    addRSSLayout->addWidget(setRSS);

    QHBoxLayout* horLayout = new QHBoxLayout();
    QVBoxLayout* rssVertLayout = new QVBoxLayout();
    horLayout->addLayout(rssVertLayout);
    rssVertLayout->addWidget(RSSListLabel);
    rssVertLayout->addWidget(RSSList);

    newsTabLayout->addLayout(addRSSLayout);
    newsTabLayout->addLayout(horLayout);


    connect(setRSS, SIGNAL(clicked()), this, SLOT(setRSSFeed()));
}

void News::setRSSFeed()
{
    QString url = rssAddress->text();
    GetRSSFeed(url);
}

void News::GetRSSFeed(QString url)
{
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    reply = manager.get(request);
    connect(reply, SIGNAL(finished()),this, SLOT(onRSSReturned()));
    qDebug() << "TestRSS";
}

void News::onRSSReturned()
{
    RSSList->clear();
    QByteArray data = reply->readAll();
    QXmlStreamReader xml(data);
    qDebug() << data;
    while(!xml.atEnd())
    {
        if(xml.isStartElement()) {
            if (xml.name() == "channel")
            {
                xml.readNext();
                if (xml.name() == "title") {
                    RSSListLabel->setText(xml.readElementText());
                }
            }
            if (xml.name() == "item")
            {
                xml.readNext();
                RSSList->addItem(xml.readElementText());
            }
        }
        xml.readNext();
    }
    xml.clear();
    qDebug() << "Reply";
    qDebug() << data;
}

void News::parseXml()
{

}

News::~News()
{
}
