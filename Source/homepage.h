#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class HomePage;
}

class HomePage : public QWidget
{
    Q_OBJECT

public:
    HomePage(QSettings* p, QWidget* parent = 0);
    ~HomePage();

private:
    Ui::HomePage *ui;
};

#endif // HOMEPAGE_H
