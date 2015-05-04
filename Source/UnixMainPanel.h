#ifndef UNIXWINPANEL_H
#define UNIXWINPANEL_H

#include "MainPanel.h"

class UnixPanel;

class UnixMainPanel : public MainPanel
{
    Q_OBJECT

public:
    UnixMainPanel(UnixPanel* panel);

public slots:
    void pushButtonMinimize();
    void pushButtonMaximize();
    void pushButtonClose();

private:
    UnixPanel* unixPanel;

};

#endif // UNIXMAINPANEL_H
