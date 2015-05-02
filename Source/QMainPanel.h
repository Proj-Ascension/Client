#ifndef QMAINPANEL_H
#define QMAINPANEL_H

#include <QMouseEvent>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QWidget>
#ifdef Q_OS_WIN
#include "QWinWidget.h"
#else
#include <QMainWindow>
#endif
#include "TabLabel.h"

#ifdef Q_OS_WIN
class QMainPanel : public QWinWidget
#else
class QMainPanel : public QWidget
#endif
{
    Q_OBJECT

public:
#ifdef Q_OS_WIN
    QMainPanel(HWND hWnd);
#else
    QMainPanel(QMainWindow* window);
#endif

#ifdef Q_OS_WIN
#if QT_VERSION >= 0x050000
    bool nativeEvent(const QByteArray &, void *msg, long *result);
#else
    bool winEvent(MSG *message, long *result);
#endif
    void mousePressEvent(QMouseEvent *event);
#endif

public slots:
    void pushButtonMinimizeClicked();
    void pushButtonMaximizeClicked();
    void pushButtonCloseClicked();
    void setTabLibrary();
    void setTabBrowser();

private:
#ifdef Q_OS_WIN
    HWND windowHandle;
#else
    QMainWindow* qWindow;
#endif
    QStackedWidget *stack;
    QWidget *libraryPtr;
    QWidget *browserPtr;

    TabLabel *activeTab;
    TabLabel *libraryTab;
    TabLabel *storeTab;
    TabLabel *modsTab;
    TabLabel *newsTab;
    TabLabel *browserTab;

protected:

};

#endif // QMAINPANEL_H
