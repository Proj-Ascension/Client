#include "MainPanel.h"

#include <QMessageBox>

TabLabel* g_tabFactory(TabLabel* label, QString name, QString text)
{
    label->setObjectName(name);
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    label->setMinimumWidth(80);
    label->setMaximumWidth(110);
    label->setAlignment(Qt::AlignTop);
    label->setFont(QFont("Sansation", 14));
    label->setText(text);
    label->setStyleSheet("color: #FFF;");

    return label;
}

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

    stack = new QStackedWidget(this);

    QString style = g_getStylesheet(":/Styles/Content.css");
    // Prepare UI objects for each tab
    libraryPtr = new Library(db);
    libraryPtr->setStyleSheet(style);
    browserPtr = new Browser();
    browserPtr->setStyleSheet(style);
    stack->addWidget(libraryPtr);
    stack->addWidget(browserPtr);
    stack->setCurrentWidget(libraryPtr);

    // System layout
    QHBoxLayout* systemLayout = new QHBoxLayout;
    systemLayout->setSpacing(0);
    systemLayout->setMargin(8);

    // Header spacing
    QVBoxLayout* topLayout = new QVBoxLayout;
    topLayout->setMargin(0);

    // Header layout
    QHBoxLayout* headerLayout = new QHBoxLayout;
    headerLayout->setSpacing(0);
    headerLayout->setMargin(0);

    // Window title
    QLabel* windowTitle = new QLabel(this);
    windowTitle->setObjectName("windowTitle");
    windowTitle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    windowTitle->setMinimumWidth(175);
    windowTitle->setMaximumWidth(175);
    windowTitle->setAlignment(Qt::AlignTop);
    windowTitle->setFont(QFont("Sansation", 18));
    windowTitle->setText("Project \nASCENSION");
    windowTitle->setStyleSheet("color: #7D818C;");
    windowTitle->setAttribute(Qt::WA_TransparentForMouseEvents);

    // Post-initialization header spacing
    topLayout->addLayout(systemLayout);
    topLayout->addLayout(headerLayout);
    topLayout->addSpacing(10);

    headerLayout->addSpacing(20);
    headerLayout->addWidget(windowTitle);
    headerLayout->addSpacing(40);

    // Header tabs
    libraryTab = new TabLabel(this);
    libraryTab = g_tabFactory(libraryTab, "libraryTab", "LIBRARY");
    headerLayout->addSpacing(8);
    headerLayout->addWidget(libraryTab);
    libraryTab->setStyleSheet("font-weight: bold; color: lightgreen;");

    storeTab = new TabLabel(this);
    storeTab = g_tabFactory(storeTab, "storeTab", "  STORE");
    headerLayout->addSpacing(8);
    headerLayout->addWidget(storeTab);

    modsTab = new TabLabel(this);
    modsTab = g_tabFactory(modsTab, "modsTab", " MODS");
    headerLayout->addSpacing(8);
    headerLayout->addWidget(modsTab);

    newsTab = new TabLabel(this);
    newsTab = g_tabFactory(newsTab, "newsTab", "NEWS");
    headerLayout->addSpacing(8);
    headerLayout->addWidget(newsTab);

    browserTab = new TabLabel(this);
    browserTab = g_tabFactory(browserTab, "browserTab", "BROWSER");
    headerLayout->addSpacing(8);
    headerLayout->addWidget(browserTab);

    activeTab = libraryTab;

    headerLayout->addStretch();

    // System buttons
    systemLayout->addStretch();

    // Minimize
    QPushButton* pushButtonMinimize = new QPushButton("", this);
    pushButtonMinimize->setObjectName("pushButtonMinimize");
    systemLayout->addWidget(pushButtonMinimize);
    QObject::connect(pushButtonMinimize, SIGNAL(clicked()), this, SLOT(pushButtonMinimize()));

    // Maximize
    QPushButton* pushButtonMaximize = new QPushButton("", this);
    pushButtonMaximize->setObjectName("pushButtonMaximize");
    systemLayout->addWidget(pushButtonMaximize);
    QObject::connect(pushButtonMaximize, SIGNAL(clicked()), this, SLOT(pushButtonMaximize()));

    // Close
    QPushButton* pushButtonClose = new QPushButton("", this);
    pushButtonClose->setObjectName("pushButtonClose");
    systemLayout->addWidget(pushButtonClose);
    QObject::connect(pushButtonClose, SIGNAL(clicked()), this, SLOT(pushButtonClose()));

    // Main panel layout
    QGridLayout* mainGridLayout = new QGridLayout();
    mainGridLayout->setSpacing(0);
    mainGridLayout->setMargin(0);
    setLayout(mainGridLayout);

    // Central widget
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");
    centralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Main panel scroll area
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setObjectName("mainPanelScrollArea");
    scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Vertical layout example
    QVBoxLayout* verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(5);
    verticalLayout->setMargin(0);
    verticalLayout->setAlignment(Qt::AlignHCenter);
    verticalLayout->addLayout(topLayout, 1);

    verticalLayout->addWidget(stack, 4);

    // Connect signals
    connect(libraryTab, SIGNAL(clicked()), this, SLOT(setTabLibrary()));
    connect(browserTab, SIGNAL(clicked()), this, SLOT(setTabBrowser()));

    // Show
    centralWidget->setLayout(verticalLayout);
    scrollArea->setWidget(centralWidget);
    mainGridLayout->addWidget(scrollArea);
}

// Tab swap slots
void MainPanel::setTabLibrary()
{
    if (stack->currentWidget()->objectName() != "libraryUI")
    {
        activeTab->setStyleSheet("font-weight: regular; color: #FFF;");
        stack->setCurrentWidget(libraryPtr);
        activeTab = libraryTab;
        activeTab->setStyleSheet("font-weight: bold; color: lightgreen;");
    }
}

void MainPanel::setTabBrowser()
{
    if (stack->currentWidget()->objectName() != "browserUI")
    {
        activeTab->setStyleSheet("font-weight: regular; color: #FFF;");
        stack->setCurrentWidget(browserPtr);
        activeTab = browserTab;
        activeTab->setStyleSheet("font-weight: bold; color: lightgreen;");
    }
}
