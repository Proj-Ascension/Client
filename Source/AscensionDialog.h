#pragma once

#include <QDialog>
#include <QSettings>

class AscensionDialog : public QDialog
{
    Q_OBJECT

public:
    AscensionDialog(QString title = "Project Ascension");
    ~AscensionDialog();

    static bool showConfirmDialog(QString title, QString message);

public slots:
    void pushButtonClose();

private:
    QSettings* p;
    QPoint oldWindowPos;
    bool dragging;

    QWidget* content;

    void mousePressEvent(QMouseEvent* evt);
    void mouseReleaseEvent(QMouseEvent* evt);
    void mouseMoveEvent(QMouseEvent* evt);
};

