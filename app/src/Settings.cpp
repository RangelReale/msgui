#include "msgui/Settings.h"

#include <QSettings>
#include <QDir>
#include <QFileInfo>
#include <QBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QLoggingCategory>

#include <log4qt/logger.h>

namespace msgui {

Settings::Settings(QWidget *parent) : QDialog(parent, Qt::MSWindowsFixedSizeDialogHint)
{
	setWindowTitle("Settings");

	QVBoxLayout *layout = new QVBoxLayout;
	setLayout(layout);

	// settings
	QFormLayout *form = new QFormLayout;
	layout->addLayout(form);

	// executable
	_executable = new QLineEdit(this);
	QPushButton *executable_find = new QPushButton("&Select", this);
	connect(executable_find, &QPushButton::clicked, this, &Settings::onBtnFindExecutable);

	QHBoxLayout *exelayout = new QHBoxLayout;
	exelayout->addWidget(_executable);
	exelayout->addWidget(executable_find);

	form->addRow(tr("Metashell executable"), exelayout);


	// buttons
	QHBoxLayout *btnlayout = new QHBoxLayout;
	layout->addLayout(btnlayout);

	QPushButton *btnOk = new QPushButton(tr("Ok"), this);
	connect(btnOk, &QPushButton::clicked, this, &Settings::onBtnOk);
	QPushButton *btnCancel = new QPushButton(tr("Cancel"), this);
	connect(btnCancel, &QPushButton::clicked, this, &Settings::onBtnCancel);

	btnlayout->addWidget(btnOk);
	btnlayout->addWidget(btnCancel);

	// read settings
	readSettings();
}

void Settings::readSettings()
{
	QSettings settings;
	_executable->setText(settings.value("metashell_program", "").toString());
}

void Settings::writeSettings()
{
	QSettings settings;
	if (!_executable->text().isEmpty()) {
		settings.setValue("metashell_program", _executable->text());
	}
	else {
		settings.remove("metashell_program");
	}
}


void Settings::onBtnFindExecutable()
{
	QString defaultPath(QDir::currentPath());
	if (!_executable->text().isEmpty()) {
		QFileInfo fi(_executable->text());
		defaultPath = fi.absoluteFilePath();
	}
	else
	{
		QString executable(findMetashellExecutable());
		if (!executable.isEmpty())
		{
			QFileInfo fi(executable);
			defaultPath = fi.absoluteFilePath();
		}
	}

	QString filename = QFileDialog::getOpenFileName(this,
		tr("Find Metashell executable"), defaultPath, "Metashell Executable (metashell.exe);");

	if (!filename.isEmpty()) {
		_executable->setText(filename);
	}
}

void Settings::onBtnOk()
{
	writeSettings();
	accept();
}

void Settings::onBtnCancel()
{
	reject();
}


QString Settings::findMetashellExecutable()
{
	QString process;

#ifdef Q_OS_WIN
	// try to find the process on the windows registry
	// 64 bit registry
	{
		QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Abel Sinkovics\\metashell 3.0.0", QSettings::Registry64Format);
		QString reg_process = settings.value("Default", "").toString();
		if (!reg_process.isEmpty()) {
			QFileInfo path(QDir(reg_process).filePath("bin/metashell.exe"));
			if (path.exists()) {
				process = path.absoluteFilePath();
			}
		}
	}

	// 32 bit registry
	if (process.isEmpty())
	{
		QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Abel Sinkovics\\metashell 3.0.0", QSettings::Registry32Format);
		QString reg_process = settings.value("Default", "").toString();
		if (!reg_process.isEmpty()) {
			QFileInfo path(QDir(reg_process).filePath("bin/metashell.exe"));
			if (path.exists()) {
				process = path.absoluteFilePath();
			}
		}
	}
#endif

	if (process.isEmpty()) {
		process = "metashell";
	}

	Log4Qt::Logger::rootLogger()->logger("settings")->info("Finding metashell executable: %1", process);

	return process;
}

}