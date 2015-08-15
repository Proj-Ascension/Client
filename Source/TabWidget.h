#pragma once

#include <QWidget>
#include <QSettings>

class TabWidget : public QWidget
{
    Q_OBJECT

public:
    TabWidget(const QPixmap &icon, const QString &name, const QString &text, const QSettings &p, QWidget* parent = 0);

    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
    void mousePressEvent(QMouseEvent* event);

protected:
    void paintEvent(QPaintEvent* event);

signals:
    void hovered();
    void unhovered();
    void clicked();
};
