#pragma once

#include <QSettings>
#include <QtNetwork>
#include <QBoxLayout>
#include <QLineEdit>
#include <QScrollArea>
#include <vector>

/** News UI namespace. */
/** News class.
* Class to handle the settings section of the launcher
*/
class News : public QWidget
{
    Q_OBJECT

public:
    explicit News(QSettings* p, QWidget* parent = 0);
    ~News();

public slots:
    void setRSSFeed();
    void onRSSReturned(QNetworkReply* reply);

private:
    QHBoxLayout* hNewsLayout;
    QLineEdit* rssAddress;
    QNetworkAccessManager* manager;
    QSettings* rss;
    std::vector<QString> urls;

    QSettings* p;

    void getRSSFeed(QString url);
    void saveFeeds(QString title, QString url);
    void loadFeeds();
};
