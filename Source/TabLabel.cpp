#include "TabLabel.h"

TabLabel::TabLabel(QWidget *parent) : QLabel(parent)
{
    // Set up drop shadow effect for hover event.
    dse = new QGraphicsDropShadowEffect();
    dse->setBlurRadius(15);
    dse->setOffset(3);
    dse->setColor(QColor(80, 222, 86, 255));

    // Test the drop shadow effect.
    this->setGraphicsEffect(dse);
}
