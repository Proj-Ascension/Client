#include <windows.h>

#include <QApplication>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QSpacerItem>
#include <QGraphicsDropShadowEffect>

#include "QMainPanel.h"
#include "TabLabel.h"
#include "Launcher.h"

#include <windowsx.h>
#include <QFontDatabase>

TabLabel* tabFactory(TabLabel* label, QString name, QString text)
{
    label->setObjectName(name);
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    label->setMinimumWidth(80);
    label->setMaximumWidth(110);
    label->setAlignment(Qt::AlignTop);
    label->setFont(QFont("Sansation", 14));
    label->setText(text);
    label->setStyleSheet("color: #FFF;");
    label->setAttribute(Qt::WA_TransparentForMouseEvents);

    return label;
}

QMainPanel::QMainPanel(HWND hWnd) : QWinWidget(hWnd)
{

    windowHandle = hWnd;

    setObjectName("mainPanel");


    // System layout
    QHBoxLayout *systemLayout = new QHBoxLayout;
    systemLayout->setSpacing(0);
    systemLayout->setMargin(8);

    // Header spacing
    QVBoxLayout *topLayout = new QVBoxLayout;
    topLayout->setMargin(0);

    // Header layout
    QHBoxLayout *headerLayout = new QHBoxLayout;
    headerLayout->setSpacing(0);
    headerLayout->setMargin(0);


    // Window title
    QLabel *windowTitle = new QLabel(this);
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
    TabLabel *libraryTab = new TabLabel(this);
    libraryTab = tabFactory(libraryTab, "libraryTab", "LIBRARY");
    headerLayout->addSpacing(8);
    headerLayout->addWidget(libraryTab);
    libraryTab->setStyleSheet("font-weight: bold; color: lightgreen;");

    TabLabel *storeTab = new TabLabel(this);
    storeTab = tabFactory(storeTab, "storeTab", " STORE");
    headerLayout->addSpacing(8);
    headerLayout->addWidget(storeTab);

    TabLabel *modsTab = new TabLabel(this);
    modsTab = tabFactory(modsTab, "modsTab", "MODS");
    headerLayout->addSpacing(8);
    headerLayout->addWidget(modsTab);

    TabLabel *newsTab = new TabLabel(this);
    newsTab = tabFactory(newsTab, "newsTab", "NEWS");
    headerLayout->addSpacing(8);
    headerLayout->addWidget(newsTab);

    TabLabel *browserTab = new TabLabel(this);
    browserTab = tabFactory(browserTab, "browserTab", "BROWSER");
    headerLayout->addSpacing(8);
    headerLayout->addWidget(browserTab);

    headerLayout->addStretch();

    // System buttons
    systemLayout->addStretch();

    // Minimize
    QPushButton *pushButtonMinimize = new QPushButton("", this);
    pushButtonMinimize->setObjectName("pushButtonMinimize");
    systemLayout->addWidget(pushButtonMinimize);
    QObject::connect(pushButtonMinimize, SIGNAL(clicked()), this, SLOT(pushButtonMinimizeClicked()));

    // Maximize
    QPushButton *pushButtonMaximize = new QPushButton("", this);
    pushButtonMaximize->setObjectName("pushButtonMaximize");
    systemLayout->addWidget(pushButtonMaximize);
    QObject::connect(pushButtonMaximize, SIGNAL(clicked()), this, SLOT(pushButtonMaximizeClicked()));

    // Close
    QPushButton *pushButtonClose = new QPushButton("", this);
    pushButtonClose->setObjectName("pushButtonClose");
    systemLayout->addWidget(pushButtonClose);
    QObject::connect(pushButtonClose, SIGNAL(clicked()), this, SLOT(pushButtonCloseClicked()));

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

    // Vertical layout example
    QVBoxLayout *verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(5);
    verticalLayout->setMargin(0);
    verticalLayout->setAlignment(Qt::AlignHCenter);
    verticalLayout->addLayout(topLayout, 1);

    // Label example
    // QLabel *label = new QLabel( centralWidget );
    // label->setObjectName( "Label" );
    // label->setText( "Hello World! This is QLabel." );
    // label->setStyleSheet( "font-size: 48px" );
    // verticalLayout->addWidget( label );

    Launcher *launcher = new Launcher();
    verticalLayout->addWidget(launcher, 4);

    // Show
    centralWidget->setLayout(verticalLayout);
    scrollArea->setWidget(centralWidget);
    mainGridLayout->addWidget(scrollArea);

    show();
}

// Button events
void QMainPanel::pushButtonMinimizeClicked()
{
    ShowWindow(parentWindow(), SW_MINIMIZE);
}

void QMainPanel::pushButtonMaximizeClicked()
{
    WINDOWPLACEMENT wp;
    wp.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(parentWindow(), &wp);
    if (wp.showCmd == SW_MAXIMIZE)
    {
        ShowWindow(parentWindow(), SW_RESTORE);
    }
    else
    {
        ShowWindow(parentWindow(), SW_MAXIMIZE);
    }
}

void QMainPanel::pushButtonCloseClicked()
{
    PostQuitMessage(0);
}

#if QT_VERSION >= 0x050000
bool QMainPanel::nativeEvent(const QByteArray &, void *msg, long *)
{
#else
bool QMainPanel::winEvent(MSG *message, long *)
{
#endif
#if QT_VERSION >= 0x050000
    MSG *message = (MSG *)msg;
#endif
    switch(message->message)
    {
    case WM_SYSKEYDOWN:
    {
        if (message->wParam == VK_SPACE)
        {
            RECT winrect;
            GetWindowRect(windowHandle, &winrect);
            TrackPopupMenu(GetSystemMenu(windowHandle, false), TPM_TOPALIGN | TPM_LEFTALIGN, winrect.left + 5, winrect.top + 5, 0, windowHandle, NULL);
            break;
        }
    }
    case WM_KEYDOWN:
    {
        if (message->wParam == VK_F5 ||
                message->wParam == VK_F6 ||
                message->wParam == VK_F7
                )
        {
            SendMessage(windowHandle, WM_KEYDOWN, message->wParam, message->lParam);
            break;
        }
    }
    }

    return false;
}

void QMainPanel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        ReleaseCapture();
        SendMessage(windowHandle, WM_NCLBUTTONDOWN, HTCAPTION, 0);
    }

    if (event->type() == QEvent::MouseButtonDblClick)
    {
        if (event->button() == Qt::LeftButton)
        {
            WINDOWPLACEMENT wp;
            wp.length = sizeof(WINDOWPLACEMENT);
            GetWindowPlacement(parentWindow(), &wp);
            if (wp.showCmd == SW_MAXIMIZE)
            {
                ShowWindow(parentWindow(), SW_RESTORE);
            }
            else
            {
                ShowWindow(parentWindow(), SW_MAXIMIZE);
            }
        }
    }
}
