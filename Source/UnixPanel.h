#ifndef UNIXPANEL
#define UNIXPANEL

#include <QStackedWidget>
#include <QWidget>
#include "TabLabel.h"

class UnixPanel : public QWidget
{
    Q_OBJECT

public:
    UnixPanel();

public slots:
    void setTabLibrary();
    void setTabBrowser();
    void pushButtonMinimize();
    void pushButtonMaximize();
    void pushButtonClose();

protected:
    QStackedWidget *stack;
    QWidget *libraryPtr;
    QWidget *browserPtr;
    QPoint oldWindowPos;

    TabLabel *activeTab;
    TabLabel *libraryTab;
    TabLabel *storeTab;
    TabLabel *modsTab;
    TabLabel *newsTab;
    TabLabel *browserTab;

    void init();

private:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

};

#endif // UNIXPANEL

