#include "Sidebar.h"

#include <QPainter>
#include <QStyleOption>
#include <QLayout>
#include <QLabel>

Sidebar::Sidebar(const QSettings &p, QWidget* parent) : QWidget(parent)
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
    userDropdown->setObjectName("userDropdown");
    userDropdown->setMinimumWidth(188);
    userDropdown->setMinimumHeight(58);
    userDropdown->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    userDropdown->setStyleSheet("QWidget#userDropdown {background-color: " + 
                                p.value("Primary/SidebarElement").toString() + ";}");
    upperLayout->addWidget(userDropdown);
    upperLayout->addStretch();

    // User dropdown layout
    QVBoxLayout* dropdownLayout = new QVBoxLayout;
    dropdownLayout->setSpacing(8);
    dropdownLayout->setMargin(8);
    userDropdown->setLayout(dropdownLayout);

    // Dropdown title
    QLabel* dropdownTitle = new QLabel(userDropdown);
    dropdownTitle->setObjectName("dropdownTitle");
    dropdownTitle->setStyleSheet("color: " + p.value("Primary/LightText").toString() + ";");
    dropdownTitle->setFont(QFont("SourceSansPro", 12, QFont::Black));
    dropdownTitle->setText("Project Ascension");
    dropdownLayout->addWidget(dropdownTitle);

    // Username layout
    QHBoxLayout* usernameLayout = new QHBoxLayout;
    usernameLayout->setSpacing(4);
    usernameLayout->setMargin(0);
    usernameLayout->setAlignment(Qt::AlignVCenter);
    dropdownLayout->addLayout(usernameLayout);

    // Status label
    QLabel* statusLabel = new QLabel(userDropdown);
    statusLabel->setObjectName("statusLabel");
    statusLabel->setMinimumSize(QSize(10, 10));
    statusLabel->setMaximumSize(QSize(10, 10));
    statusLabel->setPixmap(QPixmap(":/SystemMenu/Icons/Status_Online.png"));
    statusLabel->setScaledContents(true);
    usernameLayout->addWidget(statusLabel);

    // Username label
    QLabel* usernameLabel = new QLabel(userDropdown);
    usernameLabel->setObjectName("usernameLabel");
    usernameLabel->setStyleSheet("color: " + p.value("Primary/SubText").toString() + ";");
    usernameLabel->setFont(QFont("SourceSansPro", 10, QFont::DemiBold));
    usernameLabel->setText("username");
    usernameLayout->addWidget(usernameLabel);

    // Info box
    QWidget* infoBox = new QWidget(this);
    infoBox->setMinimumHeight(58);
    infoBox->setMaximumHeight(58);
    infoBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    infoBox->setStyleSheet("background-color: " + p.value("Primary/TertiaryBase").toString() +
                           ";border-top: 2px solid " + p.value("Accent/DarkAccent").toString() + ";");
    mainLayout->addWidget(infoBox);

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