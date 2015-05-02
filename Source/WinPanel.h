#ifndef WINPANEL_H
#define WINPANEL_H

#include <QMouseEvent>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QWidget>
#include "QWinWidget.h"
#include "TabLabel.h"
#include "UnixPanel.h"

class WinPanel : public QWinWidget, public UnixPanel
{
    Q_OBJECT

public:
    WinPanel(HWND hWnd);
#if QT_VERSION >= 0x050000
    bool nativeEvent(const QByteArray &, void *msg, long *result);
#else
    bool winEvent(MSG *message, long *result);
#endif
    void mousePressEvent(QMouseEvent *event);

public slots:
    void pushButtonMinimizeClicked();
    void pushButtonMaximizeClicked();
    void pushButtonCloseClicked();

private:
    HWND windowHandle;

};

#endif // WINPANEL_H
