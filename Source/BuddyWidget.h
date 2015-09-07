#pragma once

#include <QWidget>
#include <QSettings>
#include <QLabel>
#include <QGraphicsEffect>
#include <QPropertyAnimation>

/** BuddyWidget class.
* Class to handle the construction and behavior of sidebar tabs.
*/
class BuddyWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ getOpacity WRITE setOpacity)

public:
    BuddyWidget(const QPixmap &icon, const QString &name, const QString &text, QSettings* p, QWidget* parent = 0);

    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void toggleActive();
    void toggleInactive();

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

    QSettings* p;

signals:
    void hovered();
    void unhovered();
    void clicked();

private slots:
    void toggleHovered();
    void toggleUnhovered();
};
