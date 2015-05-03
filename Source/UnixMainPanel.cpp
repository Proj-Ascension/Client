#include "UnixMainPanel.h"

#include "UnixPanel.h"

UnixMainPanel::UnixMainPanel(UnixPanel* panel) : MainPanel((QWidget*) panel),
    unixPanel(panel)
{
    this->adjustSize();
}

void UnixMainPanel::pushButtonMinimize()
{
    unixPanel->minimize();
}

void UnixMainPanel::pushButtonMaximize()
{
    unixPanel->maximize();
}

void UnixMainPanel::pushButtonClose()
{
    unixPanel->closeWindow();
}
