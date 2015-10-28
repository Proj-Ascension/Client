#pragma once

#include "main_panel.h"

class WinWindow;

class WinPanel : public MainPanel
{
    Q_OBJECT

public:
    WinPanel(WinWindow* panel);

public slots:
    void pushButtonMinimize();
    void pushButtonMaximize();
    void pushButtonClose();

private:
    WinWindow* winPanel;

};
