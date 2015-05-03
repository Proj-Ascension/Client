#ifndef MAINPANEL_H
#define MAINPANEL_H

#include <QWidget>
#include <QStackedWidget>
#include <TabLabel.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include "Library.h"
#include "Browser.h"

class MainPanel : public QWidget
{
    Q_OBJECT

public:
    MainPanel(QWidget *parent = 0);

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

    TabLabel *activeTab;
    TabLabel *libraryTab;
    TabLabel *storeTab;
    TabLabel *modsTab;
    TabLabel *newsTab;
    TabLabel *browserTab;

    void init();
};

#endif // MAINPANEL_H
