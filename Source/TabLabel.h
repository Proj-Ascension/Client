#ifndef TABLABEL_H
#define TABLABEL_H

#include <QLabel>
#include <QEvent>
#include <QGraphicsDropShadowEffect>

class TabLabel : public QLabel
{
    Q_OBJECT

public:
    TabLabel(QWidget *parent = 0);

    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

signals:
    void hovered();
    void unhovered();

private:
    QGraphicsDropShadowEffect *dse = NULL;

private slots:
    void enableShadow();
    void disableShadow();
};

#endif // TABLABEL_H
