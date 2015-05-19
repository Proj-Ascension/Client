#pragma once

#include "Library.h"
#include "Browser.h"
#include "TabLabel.h"
#include "Database.h"

#include <QWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>

class MainPanel : public QWidget
{
    Q_OBJECT

public:
    MainPanel(QWidget* parent = 0);

public slots:
    void setTabLibrary();
    void setTabBrowser();
    virtual void pushButtonMinimize() = 0;
    virtual void pushButtonMaximize() = 0;
    virtual void pushButtonClose() = 0;

protected:
    Database db;

    QStackedWidget* stack;
    QWidget* libraryPtr;
    QWidget* browserPtr;

    TabLabel* activeTab;
    TabLabel* libraryTab;
    TabLabel* storeTab;
    TabLabel* modsTab;
    TabLabel* newsTab;
    TabLabel* browserTab;

    void init();
};
