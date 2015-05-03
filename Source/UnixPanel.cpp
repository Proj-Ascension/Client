#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QVBoxLayout>

#include "UnixPanel.h"

UnixPanel::UnixPanel()
{
    setObjectName("unixPanel");

    mainPanel = new UnixMainPanel(this);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->setMargin(0);
    setLayout(layout);
    layout->addWidget(mainPanel);

    this->setWindowFlags(Qt::FramelessWindowHint);
    mainPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    this->adjustSize();
    this->move(QApplication::desktop()->availableGeometry().center() - this->rect().center());
    this->show();
}

void UnixPanel::minimize()
{
    showMinimized();
}

void UnixPanel::maximize()
{
    if (isMaximized()) {
        showNormal();
    } else {
        showMaximized();
    }
}

void UnixPanel::closeWindow()
{
    this->close();
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
        move(x() + delta.x(), y() + delta.y());
        oldWindowPos = evt->globalPos();
    }
}
