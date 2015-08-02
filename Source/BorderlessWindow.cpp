#include "BorderlessWindow.h"

#include <QDesktopWidget>
#include <QPushButton>
#include <QFile>

#include <tchar.h>
#include <dwmapi.h>
#include <windowsx.h>
#include <stdexcept>

HWND g_winId = 0;

WinWindow* BorderlessWindow::mainPanel;
QApplication* BorderlessWindow::a;

BorderlessWindow::BorderlessWindow(QApplication* app, HBRUSH windowBackground, const int width, const int height)
    : hWnd(0),
      hInstance(GetModuleHandle(NULL)),
      closed(false),
      visible(false),
      borderless(false),
      borderlessResizeable(true),
      aeroShadow(false)
{
    WNDCLASSEX wcx = {0};
    wcx.cbSize = sizeof(WNDCLASSEX);
    wcx.style = CS_HREDRAW | CS_VREDRAW;
    wcx.hInstance = hInstance;
    wcx.lpfnWndProc = WndProc;
    wcx.cbClsExtra = 0;
    wcx.cbWndExtra = 0;
    wcx.lpszClassName = _T("WindowClass");
    wcx.hbrBackground = windowBackground;
    wcx.hCursor = LoadCursor(hInstance, IDC_ARROW);
    RegisterClassEx(&wcx);
    if (FAILED(RegisterClassEx(&wcx)))
    {
        throw std::runtime_error("Couldn't register window class");
    }

    // Center window at runtime
    QRect rec = QApplication::desktop()->screenGeometry();
    int screenWidth = rec.width(), screenHeight = rec.height();
    int offsetX = (screenWidth - width) / 2;
    int offsetY = (screenHeight - height) / 2;

    hWnd = CreateWindow(_T("WindowClass"), _T("Project Ascension"), static_cast<DWORD>(Style::windowed), offsetX, offsetY, width, height, 0, 0, hInstance, nullptr);

    if (!hWnd)
    {
        throw std::runtime_error("Encountered an unknown error while constructing the launcher window.");
    }

    SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

    mainPanel = new WinWindow(hWnd);
    g_winId = (HWND)mainPanel->winId();

    show();
    toggleBorderless();

    a = app;
}

BorderlessWindow::~BorderlessWindow()
{
    hide();
    DestroyWindow(hWnd);
}

HDC g_hdc;
PAINTSTRUCT g_ps;

LRESULT CALLBACK BorderlessWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    BorderlessWindow* window = reinterpret_cast<BorderlessWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    if (!window)
    {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    switch (message)
    {
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_F5:
        {
            window->borderlessResizeable = !window->borderlessResizeable;
            break;
        }
        case VK_F6:
        {
            window->toggleShadow();
            window->toggleBorderless();
            SetFocus(g_winId);
            break;
        }
        case VK_F7:
        {
            window->toggleShadow();
            break;
        }
        }

        if (wParam != VK_TAB)
        {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }

        SetFocus(g_winId);
        break;
    }
    case WM_SYSCOMMAND: // ALT + SPACE or F10 system menu
    {
        if (wParam == SC_KEYMENU)
        {
            RECT winrect;
            GetWindowRect(hWnd, &winrect);
            TrackPopupMenu(GetSystemMenu(hWnd, false), TPM_TOPALIGN | TPM_LEFTALIGN, winrect.left + 5, winrect.top + 5, 0, hWnd, NULL);
            break;
        }
        else
        {
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    case WM_SETFOCUS:
    {
        QString str("Got focus");
        QWidget* widget = QWidget::find((WId)HWND(wParam));
        if (widget)
        {
            str += QString(" from %1 (%2)").arg(widget->objectName()).arg(widget->metaObject()->className());
        }

        str += "\n";
        OutputDebugStringA(str.toLocal8Bit().data());
        break;
    }
    case WM_NCCALCSIZE:
    {
        // this kills the window frame and title bar we added with
        // WS_THICKFRAME and WS_CAPTION
        if (window->borderless)
        {
            return 0;
        }

        break;
    }
    case WM_KILLFOCUS:
    {
        QString str("Lost focus");
        QWidget* widget = QWidget::find((WId) HWND (wParam));
        if (widget)
        {
            str += QString(" to %1 (%2)").arg(widget->objectName()).arg(widget->metaObject()->className());
        } 
        str += "\n";

        OutputDebugStringA(str.toLocal8Bit().data());
        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    case WM_NCHITTEST:
    {
        if (window->borderless)
        {
            if (window->borderlessResizeable)
            {
                const LONG c_borderWidth = 8; //in pixels
                RECT winrect;
                GetWindowRect(hWnd, &winrect);
                long x = GET_X_LPARAM(lParam);
                long y = GET_Y_LPARAM(lParam);

                // bottom left corner
                if (x >= winrect.left && x < winrect.left + c_borderWidth &&
                        y < winrect.bottom && y >= winrect.bottom - c_borderWidth)
                {
                    return HTBOTTOMLEFT;
                }
                // bottom right corner
                if (x < winrect.right && x >= winrect.right - c_borderWidth &&
                        y < winrect.bottom && y >= winrect.bottom - c_borderWidth)
                {
                    return HTBOTTOMRIGHT;
                }
                // top left corner
                if (x >= winrect.left && x < winrect.left + c_borderWidth &&
                        y >= winrect.top && y < winrect.top + c_borderWidth)
                {
                    return HTTOPLEFT;
                }
                // top right corner
                if (x < winrect.right && x >= winrect.right - c_borderWidth &&
                        y >= winrect.top && y < winrect.top + c_borderWidth)
                {
                    return HTTOPRIGHT;
                }
                // left border
                if (x >= winrect.left && x < winrect.left + c_borderWidth)
                {
                    return HTLEFT;
                }
                // right border
                if (x < winrect.right && x >= winrect.right - c_borderWidth)
                {
                    return HTRIGHT;
                }
                // bottom border
                if (y < winrect.bottom && y >= winrect.bottom - c_borderWidth)
                {
                    return HTBOTTOM;
                }
                // top border
                if (y >= winrect.top && y < winrect.top + c_borderWidth)
                {
                    return HTTOP;
                }
            }
            return HTCAPTION;
        }
        break;
    }
    case WM_SIZE:
    {
        RECT winrect;
        GetClientRect(hWnd, &winrect);

        WINDOWPLACEMENT wp;
        wp.length = sizeof(WINDOWPLACEMENT);
        GetWindowPlacement(hWnd, &wp);
        if (wp.showCmd == SW_MAXIMIZE)
        {
            QPushButton* pushButtonMaximize = mainPanel->findChild<QPushButton*>("pushButtonMaximize");
            pushButtonMaximize->setStyleSheet("#pushButtonMaximize {image: url(:/SystemMenu/Icons/Restore.png);} #pushButtonMaximize:hover { image: url(:/SystemMenu/Icons/RestoreHover.png); }");
            mainPanel->setGeometry(8, 8, winrect.right - 16, winrect.bottom - 16);
        }
        else
        {
            QPushButton* pushButtonMaximize = mainPanel->findChild<QPushButton*>("pushButtonMaximize");
            pushButtonMaximize->setStyleSheet("#pushButtonMaximize {image: url(:/SystemMenu/Icons/Maximize.png);} #pushButtonMaximize:hover { image: url(:/SystemMenu/Icons/MaximizeHover.png); }");
            mainPanel->setGeometry(2, 2, winrect.right - 4, winrect.bottom - 4);
        }
        break;
    }
    case WM_GETMINMAXINFO:
    {
        MINMAXINFO* minMaxInfo = (MINMAXINFO*) lParam;
        if (window->minimumSize.required)
        {
            minMaxInfo->ptMinTrackSize.x = window->getMinimumWidth();;
            minMaxInfo->ptMinTrackSize.y = window->getMinimumHeight();
        }

        if (window->maximumSize.required)
        {
            minMaxInfo->ptMaxTrackSize.x = window->getMaximumWidth();
            minMaxInfo->ptMaxTrackSize.y = window->getMaximumHeight();
        }
        return 0;
    }
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void BorderlessWindow::toggleBorderless()
{
    if (visible)
    {
        Style newStyle = (borderless) ? Style::windowed : Style::aeroBorderless;
        SetWindowLongPtr(hWnd, GWL_STYLE, static_cast<LONG>(newStyle));

        borderless = !borderless;
        if (newStyle == Style::aeroBorderless)
        {
            toggleShadow();
        }
        // redraw frame
        SetWindowPos(hWnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
        show();
    }
}

void BorderlessWindow::toggleShadow()
{
    if (borderless)
    {
        aeroShadow = !aeroShadow;
        const MARGINS c_shadowOn = {1, 1, 1, 1};
        const MARGINS c_shadowOff = {0, 0, 0, 0};
        DwmExtendFrameIntoClientArea(hWnd, (aeroShadow) ? (&c_shadowOn) : (&c_shadowOff));
    }
}

void BorderlessWindow::toggleResizeable()
{
    borderlessResizeable = borderlessResizeable ? false : true;
}

bool BorderlessWindow::isResizeable()
{
    return borderlessResizeable ? true : false;
}

void BorderlessWindow::show()
{
    ShowWindow(hWnd, SW_SHOW);
    visible = true;
}

void BorderlessWindow::hide()
{
    ShowWindow(hWnd, SW_HIDE);
    visible = false;
}

bool BorderlessWindow::isVisible()
{
    return visible ? true : false;
}

// Minimum size
void BorderlessWindow::setMinimumSize(const int width, const int height)
{
    this->minimumSize.required = true;
    this->minimumSize.width = width;
    this->minimumSize.height = height;
}

bool BorderlessWindow::isSetMinimumSize()
{
    return this->minimumSize.required;
}

void BorderlessWindow::removeMinimumSize()
{
    this->minimumSize.required = false;
    this->minimumSize.width = 0;
    this->minimumSize.height = 0;
}

int BorderlessWindow::getMinimumWidth()
{
    return minimumSize.width;
}

int BorderlessWindow::getMinimumHeight()
{
    return minimumSize.height;
}

// Maximum size
void BorderlessWindow::setMaximumSize(const int width, const int height)
{
    this->maximumSize.required = true;
    this->maximumSize.width = width;
    this->maximumSize.height = height;
}

bool BorderlessWindow::isSetMaximumSize()
{
    return this->maximumSize.required;
}

void BorderlessWindow::removeMaximumSize()
{
    this->maximumSize.required = false;
    this->maximumSize.width = 0;
    this->maximumSize.height = 0;
}

int BorderlessWindow::getMaximumWidth()
{
    return maximumSize.width;
}

int BorderlessWindow::getMaximumHeight()
{
    return maximumSize.height;
}

