#ifndef WINMAINPANEL_H
#define WINMAINPANEL_H

#include "MainPanel.h"

class WinPanel;

class WinMainPanel : public MainPanel
{
    Q_OBJECT

public:
    WinMainPanel(WinPanel* panel);

public slots:
    void pushButtonMinimize();
    void pushButtonMaximize();
    void pushButtonClose();

private:
    WinPanel* winPanel;

};

#endif // WINMAINPANEL_H
