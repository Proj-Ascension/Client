#pragma once

#include <QWidget>
#include <QSettings>

class Sidebar : public QWidget
{
    Q_OBJECT

public:
    explicit Sidebar(const QSettings &settings, QWidget* parent = 0);
    virtual QSize sizeHint() const;

protected:
    void paintEvent(QPaintEvent* event);
};