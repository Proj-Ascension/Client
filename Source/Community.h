#ifndef COMMUNITY_H
#define COMMUNITY_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class Community;
}

class Community : public QWidget
{
    Q_OBJECT

public:
    explicit Community(QSettings* p, QWidget* parent = 0);
    ~Community();

private slots:
    void on_buddyButton_clicked();

private:
    Ui::Community *ui;
};

#endif // COMMUNITY_H
