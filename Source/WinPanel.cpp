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

WinPanel::WinPanel(HWND hWnd) : QWinWidget(hWnd)
{
    windowHandle = hWnd;
    mainPanel = new WinMainPanel(this);
    mainPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    setObjectName("winPanel");

    this->show();
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

void WinPanel::minimize()
{
    ShowWindow(parentWindow(), SW_MINIMIZE);
}

void WinPanel::maximize()
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

void WinPanel::close()
{
    PostQuitMessage(0);
}
