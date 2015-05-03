#include <QApplication>
#include <QDesktopWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QSpacerItem>
#include <QGraphicsDropShadowEffect>
#include <QFontDatabase>

#include "UnixPanel.h"

UnixPanel::UnixPanel()
{
    setObjectName("unixPanel");

    this->setWindowFlags(Qt::FramelessWindowHint);

    init();

    this->adjustSize();
    this->move(QApplication::desktop()->availableGeometry().center() - this->rect().center());
    this->show();
}



void UnixPanel::mousePressEvent(QMouseEvent* evt)
{
    oldWindowPos = evt->globalPos();
}

void UnixPanel::mouseMoveEvent(QMouseEvent* evt)
{
    const QPoint delta = evt->globalPos() - oldWindowPos;
    if (evt->pos().y() < 70)
    {
        move(x()+delta.x(), y()+delta.y());
        oldWindowPos = evt->globalPos();
    }
}
