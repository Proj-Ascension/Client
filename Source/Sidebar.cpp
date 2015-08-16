#include "Sidebar.h"
#include "TabWidget.h"

#include <QPainter>
#include <QStyleOption>
#include <QLayout>
#include <QLabel>

Sidebar::Sidebar(QSettings* p, QWidget* parent) : QWidget(parent)
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
    upperLayout->setSpacing(5);
    upperLayout->setMargin(18);
    mainLayout->addLayout(upperLayout);

    // User dropdown label
    QWidget* userDropdown = new QWidget(this);
    userDropdown->setObjectName("userDropdown");
    userDropdown->setMinimumWidth(188);
    userDropdown->setMinimumHeight(58);
    userDropdown->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    userDropdown->setStyleSheet("QWidget#userDropdown {background-color: " + 
                                p->value("Primary/DarkElement").toString() + ";}");
    upperLayout->addWidget(userDropdown);

    // User dropdown layout
    QVBoxLayout* dropdownLayout = new QVBoxLayout;
    dropdownLayout->setSpacing(8);
    dropdownLayout->setMargin(8);
    userDropdown->setLayout(dropdownLayout);

    // Dropdown title
    QLabel* dropdownTitle = new QLabel(userDropdown);
    dropdownTitle->setObjectName("dropdownTitle");
    dropdownTitle->setStyleSheet("color: " + p->value("Primary/LightText").toString() + ";");
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
    usernameLabel->setStyleSheet("color: " + p->value("Primary/SubText").toString() + ";");
    usernameLabel->setFont(QFont("SourceSansPro", 10, QFont::DemiBold));
    usernameLabel->setText("username");
    usernameLayout->addWidget(usernameLabel);

    // Tabs
    // Home tab
    QPixmap homePixmap(":/SystemMenu/Icons/Home_Icon.png");
    homeTab = new TabWidget(homePixmap, "homeTab", "Home", p, this);
    upperLayout->addWidget(homeTab);

    // Store tab
    QPixmap storePixmap(":/SystemMenu/Icons/Store_Icon.png");
    storeTab = new TabWidget(storePixmap, "storeTab", "Store", p, this);
    upperLayout->addWidget(storeTab);

    // Games tab
    QPixmap gamesPixmap(":/SystemMenu/Icons/Games_Icon.png");
    gamesTab = new TabWidget(gamesPixmap, "gamesTab", "Games", p, this);
    upperLayout->addWidget(gamesTab);

    // Community tab
    QPixmap communityPixmap(":/SystemMenu/Icons/Community_Icon.png");
    communityTab = new TabWidget(communityPixmap, "communityTab", "Community", p, this);
    upperLayout->addWidget(communityTab);

    // News tab
    QPixmap newsPixmap(":/SystemMenu/Icons/News_Icon.png");
    newsTab = new TabWidget(newsPixmap, "newsTab", "News", p, this);
    upperLayout->addWidget(newsTab);
    upperLayout->addStretch();

    // Downloads tab
    QPixmap downloadsPixmap(":/SystemMenu/Icons/Downloads_Icon.png");
    downloadsTab = new TabWidget(downloadsPixmap, "downloadsTab", "Downloads", p, this);
    upperLayout->addWidget(downloadsTab);

    // Settings tab
    QPixmap settingsPixmap(":/SystemMenu/Icons/Settings_Icon.png");
    settingsTab = new TabWidget(settingsPixmap, "settingsTab", "Settings", p, this);
    upperLayout->addWidget(settingsTab);

    // Exit tab
    QPixmap exitPixmap(":/SystemMenu/Icons/Exit_Icon.png");
    exitTab = new TabWidget(exitPixmap, "exitTab", "Exit", p, this);
    upperLayout->addWidget(exitTab);

    // Info box
    QWidget* infoBox = new QWidget(this);
    infoBox->setObjectName("infoBox");
    infoBox->setMinimumHeight(58);
    infoBox->setMaximumHeight(58);
    infoBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    infoBox->setStyleSheet("QWidget#infoBox "
                           "{background-color: " + p->value("Primary/TertiaryBase").toString() +
                           ";border-top: 2px solid " + p->value("Accent/DarkAccent").toString() + ";}");
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

