#pragma once


#include <mredit/Editor.h>

namespace msgui {

class TabEditor : public mredit::Editor
{
	Q_OBJECT
public:
	TabEditor(QWidget *parent = nullptr);

	void clearMarkSourceCode();
	void markSourceFile(int row = -1, int col = -1);

	const QString &currentFilename() const;
	void setCurrentFilename(const QString &filename);
private:
	int _editor_bmgroup_showpos;
	QString _currentFilename;
};

}