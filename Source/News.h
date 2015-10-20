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
    QHBoxLayout* horLayout = new QHBoxLayout();
    void getRSSFeed(QString url);
    void saveFeeds(QString title, QString url);
    void loadFeeds();
    QSettings* rss;

public slots:
    void setRSSFeed();
    void onRSSReturned();

public:
    explicit News(QSettings* p, QWidget* parent = 0);
    ~News();
};
