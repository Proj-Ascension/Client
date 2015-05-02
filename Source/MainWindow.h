#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include "QMainPanel.h"
#ifdef Q_OS_WIN
#include "QWinWidget.h"
#else
#include <QMainWindow>
#endif

#include <QApplication>
#include <QWidget>

class BorderlessWindow
{

#ifdef Q_OS_WIN
    enum class Style : DWORD
    {
        windowed = (WS_OVERLAPPEDWINDOW | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CLIPCHILDREN | WS_SYSMENU),
        aero_borderless = (WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CLIPCHILDREN)
    };
#endif

public:
#ifdef Q_OS_WIN
    HWND hWnd;
    HINSTANCE hInstance;

    BorderlessWindow(QApplication *app, HBRUSH windowBackground, const int width, const int height);
    ~BorderlessWindow();
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#else
    QMainWindow* qWindow;

    BorderlessWindow(QApplication *app, const int width, const int height);
    ~BorderlessWindow();
#endif

    void show();
    void hide();
    bool isVisible();

    void toggleBorderless();
#ifdef Q_OS_WIN
    void toggleShadow();
#endif
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
    static QMainPanel *mainPanel;

    bool closed;
    bool visible;

    bool borderless;
#ifdef Q_OS_WIN
    bool aeroShadow;
#endif
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
