#include <windows.h>

#include <QApplication>
#include <QGridLayout>
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QSpacerItem>
#include <QGraphicsDropShadowEffect>

#include "WinPanel.h"
#include "Library.h"
#include "Browser.h"

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

    return label;
}

WinPanel::WinPanel(HWND hWnd) : QWinWidget(hWnd)
{
    windowHandle = hWnd;

    setObjectName("mainPanel");

    init();
}

// Button events
void WinPanel::pushButtonMinimizeClicked()
{
    ShowWindow(parentWindow(), SW_MINIMIZE);
}

void WinPanel::pushButtonMaximizeClicked()
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

void WinPanel::pushButtonCloseClicked()
{
    PostQuitMessage(0);
}

#if QT_VERSION >= 0x050000
bool WinPanel::nativeEvent(const QByteArray &, void *msg, long *)
{
#else
bool WinPanel::winEvent(MSG *message, long *)
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

void WinPanel::mousePressEvent(QMouseEvent *event)
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
