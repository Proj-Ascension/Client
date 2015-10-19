#pragma once

#include <QtWidgets>
#include <QSettings>
#include <QtNetwork>


/** News UI namespace. */
/** News class.
* Class to handle the settings section of the launcher
*/
class NewsFeedWidget : public QWidget
{
Q_OBJECT
private:
    QListWidget* rssList;
    QLabel* feedTitle;

public slots:


public:
    void addRSSItem(QString title);
    void setRSSTitle(QString feedTitle);
    explicit NewsFeedWidget(QWidget* parent = 0);
    ~NewsFeedWidget();
};
