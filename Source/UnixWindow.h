#ifndef UNIXPANEL
#define UNIXPANEL

#include "UnixPanel.h"

class UnixWindow : public QWidget
{
    Q_OBJECT

public:
    UnixWindow();

    void minimize();
    void maximize();
    void closeWindow();

private:
    UnixPanel* mainPanel;
    QPoint oldWindowPos;

    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
};

#endif // UNIXPANEL_H
