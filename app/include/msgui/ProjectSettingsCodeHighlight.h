#pragma once

#include "msgui/Project.h"
#include "msgwidget/UpDownListWidget.h"

#include <QDialog>
#include <QLineEdit>
#include <QTabWidget>

namespace msgui {

class ProjectSettingsCodeHighlight : public QDialog
{
public:
	ProjectSettingsCodeHighlight(Project *project, QWidget *parent);
private slots:
	void btnCodeHighlightAddClicked();
	void btnCodeHighlightEditClicked();
	void btnCodeHighlightRemoveClicked();

	void onBtnOk();
	void onBtnCancel();
private:
	void readSettings();
	void writeSettings();

	Project *_project;

	msgwidget::UpDownListWidget *_codehighlight;
};

}