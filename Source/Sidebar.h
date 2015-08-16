#pragma once

#include "TabWidget.h"

#include <QWidget>
#include <QSettings>

/** Sidebar class.
* Class to handle the layout and control of the sidebar.
*/
class Sidebar : public QWidget
{
    Q_OBJECT

public:
    explicit Sidebar(QSettings* p, QWidget* parent = 0);
    virtual QSize sizeHint() const;

    TabWidget *homeTab, *storeTab, *gamesTab, *communityTab, 
              *newsTab, *downloadsTab, *settingsTab, *exitTab;

protected:
    void paintEvent(QPaintEvent* event);
};