#ifndef WINPANEL_H
#define WINPANEL_H

#include <QMouseEvent>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QWidget>
#include "QWinWidget.h"
#include "TabLabel.h"
#include "WinPanel.h"

class WinWindow : public QWinWidget
{
    Q_OBJECT

public:
    WinWindow(HWND hWnd);
#if QT_VERSION >= 0x050000
    bool nativeEvent(const QByteArray&, void* msg, long* result);
#else
    bool winEvent(MSG *message, long *result);
#endif
    void mousePressEvent(QMouseEvent *event);
    void minimize();
    void maximize();
    void closeWindow();

private:
    HWND windowHandle;
    WinPanel* mainPanel;
};

#endif // WINPANEL_H
