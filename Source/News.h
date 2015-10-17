#pragma once

#include <QtWidgets>
#include <QSettings>

/** News UI namespace. */
namespace Ui {
    class News;
}
/** News class.
* Class to handle the settings section of the launcher
*/
class News : public QWidget
{
    Q_OBJECT
private:
    Ui::News *ui;

public slots:
    void GetRSSFeed();
public:
    explicit News(QSettings* p, QWidget* parent = 0);
    ~News();
};
