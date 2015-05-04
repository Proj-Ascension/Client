#include <windows.h>

#include "WinWindow.h"
#include "QWinWidget.h"

#include <QApplication>
#include <QWidget>

class BorderlessWindow
{

    enum class Style : DWORD
    {
        windowed = (WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CLIPCHILDREN | WS_SYSMENU),
        aero_borderless = (WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CLIPCHILDREN)
    };

public:
    HWND hWnd;
    HINSTANCE hInstance;

    BorderlessWindow(QApplication *app, HBRUSH windowBackground, const int width, const int height);
    ~BorderlessWindow();
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    void show();
    void hide();
    bool isVisible();

    void toggleBorderless();
    void toggleShadow();
    void toggleResizeable();
    bool isResizeable();

    void setMinimumSize(const int width, const int height);
    bool isSetMinimumSize();
    void removeMinimumSize();
    int getMinimumHeight();
    int getMinimumWidth();

    void setMaximumSize(const int width, const int height);
    bool isSetMaximumSize();
    int getMaximumHeight();
    int getMaximumWidth();
    void removeMaximumSize();

private:
    static QApplication *a;
    static WinWindow *mainPanel;

    bool closed;
    bool visible;

    bool borderless;
    bool aeroShadow;
    bool borderlessResizeable;

    struct sizeType
    {
        sizeType() : required(false), width(0), height(0) {}
        bool required;
        int width;
        int height;
    };

    sizeType minimumSize;
    sizeType maximumSize;
};
