#pragma once

#include <QTreeWidget>

namespace msgui {

class TemplateKind : public QTreeWidget
{
	Q_OBJECT
public:
	TemplateKind(QWidget *parent);

	void addTemplateKind(const QString &name, const QString &kind, const QString &sourceLocation);
signals:
	void showFileAndLine(const QString &fileAndFile);
public slots:
	void btItemClicked(QTreeWidgetItem *item, int column);
	void columnResized(int logicalIndex, int oldSize, int newSize);
};

}
