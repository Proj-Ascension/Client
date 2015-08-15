#include "Sidebar.h"

#include <QPainter>
#include <QStyleOption>

Sidebar::Sidebar(QWidget* parent) : QWidget(parent)
{
    setMinimumWidth(224);
    setMaximumWidth(224);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    setStyleSheet("background-image: url(:/Elements/Sidebar.png);");

    show();
}

QSize Sidebar::sizeHint() const
{
    return QSize(224, 224);
}

void Sidebar::paintEvent(QPaintEvent* event)
{
    // Default paint event
    QWidget::paintEvent(event);

    // Style-aware drawing
    QStyleOption option;
    QPainter p(this);
    option.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &option, &p, this);
}