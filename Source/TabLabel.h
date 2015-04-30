#ifndef TABLABEL_H
#define TABLABEL_H

#include <QLabel>
#include <QGraphicsDropShadowEffect>

class TabLabel : public QLabel
{
    Q_OBJECT

public:
    TabLabel(QWidget *parent = 0);

private:
    QGraphicsDropShadowEffect *dse = NULL;
};

#endif // TABLABEL_H
