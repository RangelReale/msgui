#pragma once

#include "msgui/Project.h"
#include "msgui/interfaces.h"
#include "msgwidget/UpDownListWidget.h"

#include <QDialog>
#include <QLineEdit>
#include <QTabWidget>

namespace msgui {

class ProjectSettings : public QDialog
{
public:
	ProjectSettings(itf::Configuration *configuration, Project *project, QWidget *parent);
private slots:
	void btnStartupCodeAddClicked();
	void btnStartupCodeEditClicked();
	void btnStartupCodeRemoveClicked();

	void btnIncludeAddClicked();
	void btnIncludeRemoveClicked();
	void btnCompilerFlagsAddClicked();
	void btnCompilerFlagsRemoveClicked();

	void onBtnOk();
	void onBtnCancel();
private:
	itf::Configuration *_configuration;
	QString firstLine(const QString &data);

	void readSettings();
	void writeSettings();

	Project *_project;

	msgwidget::UpDownListWidget *_startupcodes;

	QTabWidget *_msparams;
	msgwidget::UpDownListWidget *_includepaths, *_compilerflags;
};

}