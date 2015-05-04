#ifndef WINMAINPANEL_H
#define WINMAINPANEL_H

#include "MainPanel.h"

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

#endif // WINMAINPANEL_H
