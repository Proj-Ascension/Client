#ifndef UNIXPANEL
#define UNIXPANEL

#include <QStackedWidget>
#include <QWidget>
#include "TabLabel.h"

class UnixPanel : public QWidget
{
    Q_OBJECT

public:
    UnixPanel();

private:
    void mousePressEvent(QMouseEvent* evt);
    void mouseMoveEvent(QMouseEvent* evt);

};

#endif // UNIXPANEL

