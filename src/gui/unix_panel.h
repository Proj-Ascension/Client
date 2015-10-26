#pragma once

#include "MainPanel.h"

class UnixWindow;

class UnixPanel : public MainPanel
{
    Q_OBJECT

public:
    UnixPanel(UnixWindow* panel);

public slots:
    void pushButtonMinimize();
    void pushButtonMaximize();
    void pushButtonClose();

private:
    UnixWindow* unixPanel;

};
