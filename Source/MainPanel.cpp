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

    // Horizontal layout
    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->setSpacing(0);
    horizontalLayout->setMargin(8);

    // Window title
    QLabel *windowTitle = new QLabel(this);
    horizontalLayout->addWidget(windowTitle);
    horizontalLayout->addStretch();

    // System buttons
    // Minimize
    QPushButton *pushButtonMinimize = new QPushButton("", this);
    pushButtonMinimize->setObjectName("pushButtonMinimize");
    horizontalLayout->addWidget(pushButtonMinimize);
    QObject::connect(pushButtonMinimize, SIGNAL(clicked()), this, SLOT(pushButtonMinimize()));

    // Maximize
    QPushButton *pushButtonMaximize = new QPushButton("", this);
    pushButtonMaximize->setObjectName("pushButtonMaximize");
    horizontalLayout->addWidget(pushButtonMaximize);
    QObject::connect(pushButtonMaximize, SIGNAL(clicked()), this, SLOT(pushButtonMaximize()));

    // Close
    QPushButton *pushButtonClose = new QPushButton("", this);
    pushButtonClose->setObjectName("pushButtonClose");
    horizontalLayout->addWidget(pushButtonClose);
    QObject::connect(pushButtonClose, SIGNAL(clicked()), this, SLOT(pushButtonClose()));

    // Main panel layout
    QGridLayout *mainGridLayout = new QGridLayout();
    mainGridLayout->setSpacing(0);
    mainGridLayout->setMargin(0);
    setLayout(mainGridLayout);

    // Central widget
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");
    centralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Main panel scroll area
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setObjectName("mainPanelScrollArea");
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Accent border
    QLabel *accentBorder = new QLabel(this);
    accentBorder->setMaximumHeight(3);
    accentBorder->setStyleSheet("background-color: " + p.value("Primary/DarkestBase").toString() +
                                ";border-top: 2px solid " + p.value("Accent/MediumAccent").toString() +
                                ";border-bottom: 1px solid" + p.value("Accent/DarkAccent").toString() + ";");

    // Vertical layout example
    QVBoxLayout *verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(0);
    verticalLayout->setMargin(0);
    verticalLayout->setAlignment(Qt::AlignHCenter);

    verticalLayout->addWidget(accentBorder);
    verticalLayout->addLayout(horizontalLayout);

    // Label example
    QLabel *label = new QLabel(centralWidget);
    verticalLayout->addStretch();
    verticalLayout->addWidget(label);

    // Show
    centralWidget->setLayout(verticalLayout);
    scrollArea->setWidget(centralWidget);
    mainGridLayout->addWidget(scrollArea);

    show();
}
