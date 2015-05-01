#ifndef TABLABEL_H
#define TABLABEL_H

#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>

class TabLabel : public QLabel
{
    Q_OBJECT

public:
    TabLabel(QWidget *parent = 0);

    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);

signals:
    void hovered();
    void unhovered();
    void clicked();

private:
    QGraphicsDropShadowEffect *dse = NULL;

private slots:
    void enableShadow();
    void disableShadow();
};

#endif // TABLABEL_H
