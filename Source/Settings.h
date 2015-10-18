#pragma once
#include "Database.h"

#include <QtWidgets>
#include <QSettings>

/** Settings UI namespace. */
/*namespace Ui {
    class Settings;
}*/
/** Settings class.
* Class to handle the settings section of the launcher
*/
class Settings : public QWidget
{
    Q_OBJECT
private:
    Database db;
    QPushButton* accentButton_1;
    QPushButton* accentButton_2;
    QPushButton* accentButton_3;
    QPushButton* textColourButton_1;
    QPushButton* textColourButton_2;
    QPushButton* textColourButton_3;

public slots:
    void rerunGameWizard();
    void clearDatabase();
    void setLightAccent();
    void setMediumAccent();
    void setDarkAccent();
    void resetAccents();
    void updateAccent(int accent, QColor color);
public:
    explicit Settings(QSettings* p, QWidget* parent = 0);
    ~Settings();
};
