#pragma once

#include <QSettings>
#include <QtNetwork>
#include <QBoxLayout>
#include <QLineEdit>
<<<<<<< HEAD:Source/News.h
#include <QScrollArea>
#include <vector>
=======
>>>>>>> 958508bd6d25d611800c842874b1e1a170720855:src/gui/stack/news.h

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
<<<<<<< HEAD:Source/News.h
    std::vector<QString> urls;

=======
    QSettings* p;
>>>>>>> 958508bd6d25d611800c842874b1e1a170720855:src/gui/stack/news.h

    void getRSSFeed(QString url);
    void saveFeeds(QString title, QString url);
    void loadFeeds();
};
