#ifndef UNIXPANEL
#define UNIXPANEL

#include "UnixMainPanel.h"

class UnixPanel : public QWidget
{
    Q_OBJECT

public:
    UnixPanel();

    void minimize();
    void maximize();
    void closeWindow();

private:
    QPoint oldWindowPos;
    UnixMainPanel* mainPanel;

    void mousePressEvent(QMouseEvent* evt);
    void mouseMoveEvent(QMouseEvent* evt);
};

#endif // UNIXPANEL

