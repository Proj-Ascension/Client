#pragma once

#include "TabLabel.h"
#include "Database.h"

#include <QStackedWidget>

class MainPanel : public QWidget
{
    Q_OBJECT

public:
    MainPanel(QWidget* parent = 0);

public slots:
    virtual void pushButtonMinimize() = 0;
    virtual void pushButtonMaximize() = 0;
    virtual void pushButtonClose() = 0;

protected:
    Database db;

    QStackedWidget* stack;
    QWidget* libraryPtr;

    TabLabel* activeTab;
    TabLabel* libraryTab;

    void init();
};
