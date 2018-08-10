#pragma once

#include "msgui/interfaces.h"

#include <mredit/Editor.h>

namespace msgui {

class TabEditor : public mredit::Editor
{
	Q_OBJECT
public:
	TabEditor(itf::Configuration *configuration, QWidget *parent = nullptr);

	void clearMarkSourceCode();
	void markSourceFile(int row = -1, int col = -1);

	const QString &currentFilename() const;
	void setCurrentFilename(const QString &filename);

	void openFileIfChanged(const QString &filename);
	void setPlainTextResetSize(const QString &text);
public slots:
	void onProjectChanged();
private:
	itf::Configuration *_configuration;
	int _editor_bmgroup_showpos;
	QString _currentFilename;
	quint64 _currentFilesize;
};

}