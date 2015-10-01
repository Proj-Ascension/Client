#pragma once
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
private:
	Ui::Settings *ui;

private slots:
	void on_WizardButton_clicked();

public:
	explicit Settings(QSettings* p, QWidget* parent = 0);
	~Settings();
};

