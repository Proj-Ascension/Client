#pragma once

#include <QtWidgets>
#include <QSettings>
#include <QtNetwork>


/** News UI namespace. */
/** News class.
* Class to handle the settings section of the launcher
*/
class News : public QWidget
{
    Q_OBJECT
private:
    int connectionId;
    QXmlStreamReader xml;
    QNetworkAccessManager manager;
    QNetworkReply* reply;
    QListWidget* RSSList;
    QLabel* RSSListLabel;
    QLineEdit* rssAddress;

public slots:
    void GetRSSFeed(QString url);
    void parseXml();
    void onRSSReturned();
    void setRSSFeed();
public:
    explicit News(QSettings* p, QWidget* parent = 0);
    ~News();
};
