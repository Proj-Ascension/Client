#include "TabWidget.h"

#include <QLayout>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>

TabWidget::TabWidget(const QPixmap &icon, const QString &name, const QString &text, const QSettings &p, QWidget* parent) : QWidget(parent)
{
    this->setObjectName(name);
    this->setMaximumHeight(34);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Toggle mouse tracking
    this->setAttribute(Qt::WA_Hover, true);
    this->setMouseTracking(true);
    
    // Horizontal layout
    QHBoxLayout* tabLayout = new QHBoxLayout;
    tabLayout->setMargin(5);
    tabLayout->setSpacing(10);
    this->setLayout(tabLayout);

    // Tab icon
    tabIcon = new QLabel(this);
    tabIcon->setObjectName("tabIcon");
    tabIcon->setMinimumSize(QSize(18, 18));
    tabIcon->setMaximumSize(QSize(18, 18));
    tabIcon->setScaledContents(true);
    tabIcon->setPixmap(icon);
    tabLayout->addWidget(tabIcon);

    // Tab text
    tabText = new QLabel(this);
    tabText->setObjectName("tabText");
    tabText->setStyleSheet("color: " + p.value("Primary/InactiveSelection").toString() + ";");
    tabText->setFont(QFont("SourceSansPro", 10));
    tabText->setText(text);
    tabLayout->addWidget(tabText);

    // Colorize effect
    opacity = 0;
    effect = new QGraphicsColorizeEffect();
    effect->setColor(QColor(p.value("Primary/HoverSelection").toString()));
    effect->setStrength(opacity);
    this->setGraphicsEffect(effect);

    // Colorize animation
    animation = new QPropertyAnimation(this, "opacity");
    animation->setDuration(150);
    animation->setEasingCurve(QEasingCurve::Linear);

    // Effect signals
    connect(this, SIGNAL(hovered()), this, SLOT(toggleHovered()));
    connect(this, SIGNAL(unhovered()), this, SLOT(toggleUnhovered()));
}

void TabWidget::enterEvent(QEvent* event)
{
    Q_EMIT hovered();
    QWidget::enterEvent(event);
}

void TabWidget::leaveEvent(QEvent* event)
{
    Q_EMIT unhovered();
    QWidget::leaveEvent(event);
}

void TabWidget::mousePressEvent(QMouseEvent* event)
{
    Q_EMIT clicked();
    QWidget::mousePressEvent(event);
}

void TabWidget::paintEvent(QPaintEvent* event)
{
    // Default paint event
    QWidget::paintEvent(event);

    // Style-aware drawing
    QStyleOption option;
    QPainter p(this);
    option.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &option, &p, this);
}

void TabWidget::toggleHovered()
{
    if (!isActive)
    {
        animation->stop();
        animation->setStartValue(opacity);
        animation->setEndValue(1.0);
        animation->start();
    }
}

void TabWidget::toggleUnhovered()
{
    if (!isActive)
    {
        animation->stop();
        animation->setStartValue(opacity);
        animation->setEndValue(0.0);
        animation->start();
    }
}