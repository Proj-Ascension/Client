#include "TabLabel.h"

TabLabel::TabLabel(QWidget* parent) : QLabel(parent)
{
    // Set up drop shadow effect for hover event.
    dse = new QGraphicsDropShadowEffect();
    dse->setBlurRadius(15);
    dse->setOffset(0);
    dse->setColor(QColor(80, 222, 86, 255));

    // Toggle mouse tracking
    this->setAttribute(Qt::WA_Hover, true);
    this->setMouseTracking(true);

    // Disable effect by default
    this->setGraphicsEffect(dse);
    dse->setEnabled(false);

    // Connect shadowing signals
    connect(this, SIGNAL(hovered()), this, SLOT(enableShadow()));
    connect(this, SIGNAL(unhovered()), this, SLOT(disableShadow()));
}

void TabLabel::enterEvent(QEvent* event)
{
    Q_EMIT hovered();
    QWidget::enterEvent(event);
}

void TabLabel::leaveEvent(QEvent* event)
{
    Q_EMIT unhovered();
    QWidget::leaveEvent(event);
}

void TabLabel::mousePressEvent(QMouseEvent* event)
{
    Q_EMIT clicked();
    dse->setEnabled(false);
    QWidget::mousePressEvent(event);
}

void TabLabel::enableShadow()
{
    if (this->styleSheet().contains("color: #FFF;"))
    {
        dse->setEnabled(true);
        this->setStyleSheet("color: #FFF; font-weight: bold;");
    }
}

void TabLabel::disableShadow()
{
    if (this->styleSheet().contains("color: #FFF;"))
    {
        dse->setEnabled(false);
        this->setStyleSheet("color: #FFF; font-weight: regular;");
    }
}
