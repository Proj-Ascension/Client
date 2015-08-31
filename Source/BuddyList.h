#ifndef BUDDYLIST_H
#define BUDDYLIST_H

#include <QMainWindow>

namespace Ui {
class BuddyList;
}

class BuddyList : public QMainWindow
{
    Q_OBJECT

public:
    explicit BuddyList(QWidget *parent = 0);
    ~BuddyList();

private slots:
    void on_close_clicked();

private:
    Ui::BuddyList *ui;
};

#endif // BUDDYLIST_H
