#include "Sidebar.h"

#include <QPainter>
#include <QStyleOption>
#include <QLayout>
#include <QLabel>

Sidebar::Sidebar(QWidget* parent) : QWidget(parent)
{
    setObjectName("sidebar");

    setMinimumWidth(224);
    setMaximumWidth(224);
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    setStyleSheet("Sidebar#sidebar {background-image: url(:/Elements/Sidebar.png);}");

    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    setLayout(mainLayout);

    // Upper layout
    QVBoxLayout* upperLayout = new QVBoxLayout;
    upperLayout->setSpacing(6);
    upperLayout->setMargin(18);
    mainLayout->addLayout(upperLayout);

    // User dropdown label
    QWidget* userDropdown = new QWidget(this);
    userDropdown->setMinimumWidth(188);
    userDropdown->setMinimumHeight(58);
    userDropdown->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    userDropdown->setStyleSheet("background-color: #4D5250;");
    upperLayout->addWidget(userDropdown);
    upperLayout->addStretch();

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