#ifndef PROJECTASCENSION_SETTINGS_H
#define PROJECTASCENSION_SETTINGS_H

#include "Database.h"

#include <QtWidgets>
#include <QSettings>

/** Settings UI namespace. */
namespace Ui {
    class Settings;
}
/** Settings class.
* Class to handle the settings section of the launcher
*/
class Settings : public QWidget
{
    Q_OBJECT
private:
    Ui::Settings *ui;
    Database db;

public slots:
	void on_WizardButton_clicked();
	void on_ClearDatabaseButton_clicked();
    void on_AccentButton_clicked();
    void on_AccentButton_2_clicked();
    void on_AccentButton_3_clicked();
    void on_ResetAccents_clicked();
    void updateAccent(int accent, QColor color);
public:
    explicit Settings(QSettings* p, QWidget* parent = 0);
    ~Settings();
};

#endif
