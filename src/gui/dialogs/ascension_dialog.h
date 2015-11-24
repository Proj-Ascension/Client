#pragma once

#include <QDialog>
#include <QSettings>

/** AscensionDialog class.
 * Class that extends QDialog to provide the custom UI elements for Project Ascension.
 * Use this instead of QDialog itself.
 */
class AscensionDialog : public QDialog
{
    Q_OBJECT

public:
    AscensionDialog(QString title = "Project Ascension", QWidget* parent = 0);
    ~AscensionDialog();

    static bool showConfirmDialog(QString title, QString message, QWidget* parent = 0);

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

