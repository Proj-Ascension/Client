#ifndef BUDDYLIST_H
#define BUDDYLIST_H

#include <QWidget>

namespace Ui {
class BuddyList;
}

class BuddyList : public QWidget
{
    Q_OBJECT

public:
    explicit BuddyList(QWidget *parent = 0);
    ~BuddyList();

private:
    Ui::BuddyList *ui;
};

#endif // BUDDYLIST_H
