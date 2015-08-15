#pragma once

#include <QWidget>
#include <QSettings>
#include <QLabel>
#include <QGraphicsEffect>
#include <QPropertyAnimation>

class TabWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ getOpacity WRITE setOpacity)

public:
    TabWidget(const QPixmap &icon, const QString &name, const QString &text, const QSettings &p, QWidget* parent = 0);

    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
    void mousePressEvent(QMouseEvent* event);
    int currentState = 0;

    qreal getOpacity()
    {
        return opacity;
    }
    void setOpacity(qreal val)
    {
        opacity = val;
        effect->setStrength(opacity);
        this->update();
    }

protected:
    void paintEvent(QPaintEvent* event);

private:
    QLabel* tabIcon;
    QLabel* tabText;
    QGraphicsColorizeEffect* effect;
    QPropertyAnimation* animation;
    qreal opacity;
    bool isActive = false;

signals:
    void hovered();
    void unhovered();
    void clicked();

public slots:
    void toggleActive();
    void toggleInactive();

private slots:
    void toggleHovered();
    void toggleUnhovered();
};
