#ifndef QMAINPANEL_H
#define QMAINPANEL_H

#include <QMouseEvent>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QWidget>
#include "QWinWidget.h"
#include "TabLabel.h"

class QMainPanel : public QWinWidget
{
    Q_OBJECT

public:
    QMainPanel(HWND hWnd);
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
    void setTabLibrary();
    void setTabBrowser();

private:
    HWND windowHandle;
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
