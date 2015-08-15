#pragma once

#include <QWidget>

class Sidebar : public QWidget
{
    Q_OBJECT

public:
    explicit Sidebar(QWidget* parent = 0);
    virtual QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent* event);
};