#pragma once

#include <QWidget>

namespace Ui
{
class Browser;
}

class Browser : public QWidget
{
    Q_OBJECT

public:
    explicit Browser(QWidget* parent = 0);
    ~Browser();

private:
    Ui::Browser* ui;
};
