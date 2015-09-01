#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class Homepage;
}

class Homepage : public QWidget
{
    Q_OBJECT

public:
    Homepage(QSettings* p, QWidget* parent = 0);
    ~Homepage();

private:
    Ui::Homepage* ui;
};

#endif // HOMEPAGE_H
