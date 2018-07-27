#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>

namespace msgui {

class Settings : public QDialog
{
	Q_OBJECT
public:
	Settings(QWidget *parent);

	static QString findMetashellExecutable();
private slots:
	void onBtnFindExecutable();

	void onBtnOk();
	void onBtnCancel();
private:
	void readSettings();
	void writeSettings();

	QLineEdit *_executable;
};

}