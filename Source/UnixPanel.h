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
    UnixMainPanel* mainPanel;
    QPoint oldWindowPos;

    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
};

#endif // UNIXPANEL_H
