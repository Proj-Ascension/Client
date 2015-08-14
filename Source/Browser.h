#pragma once

#include <QWidget>

/* Namespace containing the Browser UI */
namespace Ui
{
class Browser;
}

/** WebKit Browser class.
 * A simple QWebKit browser of the sub-reddit (Soon to be deprecated).
 *
 * Inherits: [QWidget](http://doc.qt.io/qt-5/qwidget.html)
 */
class Browser : public QWidget
{
    Q_OBJECT 

public:
    explicit Browser(QWidget* parent = 0);
    ~Browser();

private:
    Ui::Browser* ui;
};
