#pragma once

#include "msgui/interfaces.h"
#include "msgui/Project.h"
#include "msgwidget/UpDownListWidget.h"

namespace msgui {

class StartupCode : public msgwidget::UpDownListWidget
{
	Q_OBJECT
public:
	StartupCode(itf::Configuration *configuration, QWidget *parent);

	void loadProject(Project *project);
signals:
	void changed();
	void showCode(const QString &code);
private slots:
	void btnStartupCodeAddClicked();
	void btnStartupCodeEditClicked();
	void btnStartupCodeRemoveClicked();
	void onItemClicked(QListWidgetItem*);
	void onItemDoubleClicked(QListWidgetItem*);
	void onItemPositionChanged();
private:
	itf::Configuration *_configuration;

	void writeSettings();
	QString firstLine(const QString &data);

	Project * _project;
};

}
