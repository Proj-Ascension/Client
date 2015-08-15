#include "MainPanel.h"

#include <QMessageBox>
#include <QGridLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QSettings>

QString g_getStylesheet(QString location)
{
    QFile stylesheet(location);
    if (stylesheet.open(QFile::ReadOnly))
    {
        QString styleSheet = stylesheet.readAll();
        return styleSheet;
    }
    return "";
}

MainPanel::MainPanel(QWidget* parent)
    : QWidget(parent)
{
    setObjectName("mainPanel");

    init();
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    show();
}

void MainPanel::init()
{
    if (!db.init())
    {
        QMessageBox error;
        error.critical(0, "Error!", "An error occured while trying to load the database.");
        exit(EXIT_FAILURE);
        return;
    }

    QString style = g_getStylesheet(":/Styles/Content.css");
    QSettings p("palette.ini", QSettings::IniFormat);

    setObjectName("mainPanel");
    setStyleSheet("background-color: " + p.value("Primary/DarkestBase").toString() + ";");

    // Main panel layout
    QGridLayout *mainGridLayout = new QGridLayout(this);
    mainGridLayout->setSpacing(0);
    mainGridLayout->setMargin(0);
    setLayout(mainGridLayout);

    // Main panel scroll area
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setObjectName("mainPanelScrollArea");
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainGridLayout->addWidget(scrollArea);

    // Core widget
    QWidget *coreWidget = new QWidget(this);
    coreWidget->setObjectName("centralWidget");
    coreWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setWidget(coreWidget);

    // Vertical layout #1
    QVBoxLayout *verticalLayout1 = new QVBoxLayout(this);
    verticalLayout1->setSpacing(0);
    verticalLayout1->setMargin(0);
    verticalLayout1->setAlignment(Qt::AlignHCenter);
    coreWidget->setLayout(verticalLayout1);

    // Accent border
    QLabel *accentBorder = new QLabel(this);
    accentBorder->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    accentBorder->setMaximumHeight(3);
    accentBorder->setStyleSheet("background-color: " + p.value("Primary/DarkestBase").toString() +
                                ";border-top: 2px solid " + p.value("Accent/MediumAccent").toString() +
                                ";border-bottom: 1px solid" + p.value("Accent/DarkAccent").toString() + ";");
    accentBorder->adjustSize();
    verticalLayout1->addWidget(accentBorder);

    // Horizontal layout #1
    QHBoxLayout *horizontalLayout1 = new QHBoxLayout(this);
    horizontalLayout1->setSpacing(0);
    horizontalLayout1->setMargin(0);
    horizontalLayout1->setAlignment(Qt::AlignVCenter);
    verticalLayout1->addLayout(horizontalLayout1);

    // Sidebar widget - locked width
    QWidget *sidebar = new QWidget(this);
    sidebar->setMinimumWidth(224);
    sidebar->setMaximumWidth(224);
    sidebar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    sidebar->setStyleSheet("background-image: url(:/Elements/Sidebar.png);");
    horizontalLayout1->addWidget(sidebar);

    // Vertical layout #3
    QVBoxLayout *verticalLayout3 = new QVBoxLayout(this);
    verticalLayout3->setSpacing(0);
    verticalLayout3->setMargin(0);
    verticalLayout3->setAlignment(Qt::AlignHCenter);
    horizontalLayout1->addLayout(verticalLayout3);

    // Horizontal layout #2 - window controls
    QHBoxLayout *horizontalLayout2 = new QHBoxLayout(this);
    horizontalLayout2->setSpacing(0);
    horizontalLayout2->setMargin(8);
    verticalLayout3->addLayout(horizontalLayout2);

    // Window title
    QLabel *windowTitle = new QLabel(this);
    windowTitle->setText("Project Ascension");
    windowTitle->setStyleSheet("font-size: 16px; color: #444444;");
    windowTitle->setAttribute(Qt::WA_TransparentForMouseEvents);
    horizontalLayout2->addWidget(windowTitle);
    horizontalLayout2->addStretch();

    // Window controls
    // Minimize
    QPushButton *pushButtonMinimize = new QPushButton("", this);
    pushButtonMinimize->setObjectName("pushButtonMinimize");
    horizontalLayout2->addWidget(pushButtonMinimize);
    QObject::connect(pushButtonMinimize, SIGNAL(clicked()), this, SLOT(pushButtonMinimize()));

    // Maximize
    QPushButton *pushButtonMaximize = new QPushButton("", this);
    pushButtonMaximize->setObjectName("pushButtonMaximize");
    horizontalLayout2->addWidget(pushButtonMaximize);
    QObject::connect(pushButtonMaximize, SIGNAL(clicked()), this, SLOT(pushButtonMaximize()));

    // Close
    QPushButton *pushButtonClose = new QPushButton("", this);
    pushButtonClose->setObjectName("pushButtonClose");
    horizontalLayout2->addWidget(pushButtonClose);
    QObject::connect(pushButtonClose, SIGNAL(clicked()), this, SLOT(pushButtonClose()));

    // Stacked content panel
    QStackedWidget *stack = new QStackedWidget(this);
    stack->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    verticalLayout3->addWidget(stack);

    // Show
    show();
}
