#include "WinMainPanel.h"
#include "WinPanel.h"

WinMainPanel::WinMainPanel(WinPanel* panel) : MainPanel((QWidget*) panel),
    winPanel(panel)
{
    this->adjustSize();
}

// Button events
void WinMainPanel::pushButtonMinimize()
{
    winPanel->minimize();
}

void WinMainPanel::pushButtonMaximize()
{
    winPanel->maximize();
}

void WinMainPanel::pushButtonClose()
{
    winPanel->closeWindow();
}
