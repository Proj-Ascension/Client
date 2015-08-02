#pragma once

#include "QWinWidget.h"
#include "WinPanel.h"
#include "TabLabel.h"

class WinWindow : public QWinWidget
{
    Q_OBJECT

public:
    WinWindow(HWND hWnd);
#if QT_VERSION >= 0x050000
    bool nativeEvent(const QByteArray&, void* msg, long* result);
#else
    bool winEvent(MSG* message, long* result);
#endif
    void mousePressEvent(QMouseEvent* event);
    void minimize();
    void maximize();
    void closeWindow();

private:
    HWND windowHandle;
    WinPanel* mainPanel;
};
