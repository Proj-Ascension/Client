#pragma once

#include <QWidget>

class TabWidget : public QWidget
{
    Q_OBJECT

public:
    TabWidget(QWidget* parent = 0);

    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
    void mousePressEvent(QMouseEvent* event);

signals:
    void hovered();
    void unhovered();
    void clicked();
};
