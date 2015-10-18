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
    QPushButton* accentButton;
    QPushButton* accentButton_2;
    QPushButton* accentButton_3;

public slots:
	void on_WizardButton_clicked();
	void on_ClearDatabaseButton_clicked();
    void on_accentButton_clicked();
    void on_accentButton_2_clicked();
    void on_accentButton_3_clicked();
    void on_resetAccents_clicked();
    void updateaccent(int accent, QColor color);
public:
    explicit Settings(QSettings* p, QWidget* parent = 0);
    ~Settings();
};
