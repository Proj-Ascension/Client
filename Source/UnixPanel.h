#ifndef UNIXPANEL
#define UNIXPANEL

#include "MainPanel.h"

class UnixPanel : public QWidget
{
    Q_OBJECT

public:
    UnixPanel();

private:
    QPoint oldWindowPos;
    MainPanel* mainPanel;

    void mousePressEvent(QMouseEvent* evt);
    void mouseMoveEvent(QMouseEvent* evt);
};

#endif // UNIXPANEL

